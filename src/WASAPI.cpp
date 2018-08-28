/* An implementation file for: */
#include "api/wasapi.hpp"

/* Local libraries: */
#include "tools.hpp"
#include "logger.hpp"

/* Windows libraries: */
// For more info see doc/WASAPI_notes.md
// Property System:
//#include <propsys.h> // TOCH: Needed?
// Events:
#include <synchapi.h>

#include <iostream> //tmp
#include <limits> //tmp?
#include <thread> //tmp?
#include <chrono> //tmp?
#include <cstring> //tmp?
#include <ksmedia.h> //tmp?

/* -- Temporary testing: ---------------------------------------------------- */

float Cynth::wave_f(float t) {
    //return 1;
    //return std::sin(t);
    //return (1 / M_PI) * std::fmod(t, M_PI);
    return 0.5 * std::sin(t) + 0.5;
}
long Cynth::sin_tmp(long max, float freq, float t) {
    return max / 2 * Cynth::wave_f(t * freq * 2 * M_PI);
    //return std::sin(t * freq * 2 * M_PI);
}

float Cynth::sin_float(float freq, float t) {
    return std::sin(t * freq * 2 * M_PI);
    //return std::sin(t * freq * 2 * M_PI);
}

Cynth::Sample::Sample() {}

Cynth::Sample::Sample(int bit_depth): bit_depth(bit_depth) {
    if (bit_depth % 8 != 0)
        Cynth::Logger::errCynth("Wrong bit depth.");
    this->bytes.resize(bit_depth / 8);
}

Cynth::Sample& Cynth::Sample::operator=(unsigned long value) {
    std::memcpy(this->bytes.data(), &value, this->bytes.size());
    return *this;
}

Cynth::Sample& Cynth::Sample::operator=(long value) {
    value <<= (sizeof(value) - this->bytes.size()) * 8;
    std::memcpy(this->bytes.data(), &value, this->bytes.size());
    return *this;
}

Cynth::Sample& Cynth::Sample::operator=(float value) {
    if (sizeof(value) != this->bytes.size())
        Cynth::Logger::errCynth(
            "Unnable to convert a value to the sample bit depth.");
    std::memcpy(this->bytes.data(), &value, this->bytes.size());
    return *this;
}

Cynth::Sample& Cynth::Sample::operator=(double value) {
    Cynth::Logger::log(std::to_string(sizeof(value)));
    if (sizeof(value) != this->bytes.size())
        Cynth::Logger::errCynth(
            "Unnable to convert a value to the sample bit depth.");
    std::memcpy(this->bytes.data(), &value, this->bytes.size());
    return *this;
}

unsigned char Cynth::Sample::operator[](int i) {
    return this->bytes[i];
}

unsigned char* Cynth::Sample::data() {
    return this->bytes.data();
}

std::size_t Cynth::Sample::size() {
    return this->bytes.size();
}

Cynth::Buffer::Buffer(int bit_depth): bit_depth(bit_depth) {
    if (bit_depth % 8 != 0)
        Cynth::Logger::errCynth("Wrong bit depth.");
}

void Cynth::Buffer::write(Cynth::Sample sample) {
    for(int i = 0; i < this->bit_depth / 8; i++) {
        this->bytes.push_back(sample[i]);
    }
}

unsigned char* Cynth::Buffer::data() {
    return this->bytes.data();
}

std::size_t Cynth::Buffer::size() {
    return this->bytes.size();
}

void Cynth::Buffer::clear() {
    this->bytes.clear();
}

void Cynth::Buffer::copyTo(unsigned char* ptr_buffer) {
    std::memcpy(ptr_buffer, this->data(), this->size());
}

void Cynth::Buffer::moveTo(unsigned char* ptr_buffer) {
    this->copyTo(ptr_buffer);
    this->clear();
}

/* -- Fix missing PKEYs: ---------------------------------------------------- */

PROPERTYKEY PKEY_Device_FriendlyName;

void Cynth::WASAPI::Control::fixMissingPKEYs() {
    GUID IDevice_FriendlyName = {
        0xa45c254e, 0xdf1c, 0x4efd,
        {0x80, 0x20, 0x67, 0xd1, 0x46, 0xa8, 0x50, 0xe0}};
    PKEY_Device_FriendlyName.pid = 14;
    PKEY_Device_FriendlyName.fmtid = IDevice_FriendlyName;
}

/* -- Generic Interface: ---------------------------------------------------- */

template<typename interface_t>
Cynth::WASAPI::Interface<interface_t>::Interface()
    : ptr_instance(nullptr), auto_release(true) {}

template<typename interface_t>
Cynth::WASAPI::Interface<interface_t>::Interface(interface_t* ptr_instance)
    : ptr_instance(ptr_instance), auto_release(true) {}

template<typename interface_t>
Cynth::WASAPI::Interface<interface_t>::~Interface() {
    if (this->auto_release && this->ptr_instance) {
        this->ptr_instance->Release();
        this->ptr_instance = nullptr;
    }
}

template<typename interface_t>
void Cynth::WASAPI::Interface<interface_t>::autoRelease() {
    this->auto_release = true;
}

template<typename interface_t>
void Cynth::WASAPI::Interface<interface_t>::noAutoRelease() {
    this->auto_release = false;
}

template<typename interface_t>
interface_t* Cynth::WASAPI::Interface<interface_t>::operator->() {
    return this->ptr_instance;
}

template<typename interface_t>
Cynth::WASAPI::Interface<interface_t>::Interface(
    const Cynth::WASAPI::Interface<interface_t>& other) {
    
    Cynth::Logger::errCynth("An interface may not be copied.");
}

template<typename interface_t>
Cynth::WASAPI::Interface<interface_t>&
Cynth::WASAPI::Interface<interface_t>::operator=(
    const Cynth::WASAPI::Interface<interface_t>& other) {

    Cynth::Logger::errCynth("An interface may not be copied.");
    return *this;
}

template<typename interface_t>
Cynth::WASAPI::Interface<interface_t>::Interface(
    Cynth::WASAPI::Interface<interface_t>&& other)
    = default;

template<typename interface_t>
Cynth::WASAPI::Interface<interface_t>&
Cynth::WASAPI::Interface<interface_t>::operator=(
    Cynth::WASAPI::Interface<interface_t>&& other)
    = default;

/* -- Device Enumerator: ---------------------------------------------------- */

Cynth::WASAPI::DeviceEnumerator::DeviceEnumerator() {
    const CLSID CLSID_MMDeviceEnumerator = __uuidof(MMDeviceEnumerator);
    const IID IID_MMDeviceEnumerator = __uuidof(IMMDeviceEnumerator);
    HRESULT hr = CoCreateInstance(
        CLSID_MMDeviceEnumerator,
        NULL,
        CLSCTX_ALL,
        IID_MMDeviceEnumerator,
        (void**) &this->ptr_instance);
    if (FAILED(hr))
        Cynth::Logger::errHRESULT(hr);
}

Cynth::WASAPI::DeviceCollection
Cynth::WASAPI::DeviceEnumerator::getDeviceCollection(
    EDataFlow data_flow, // = EDataFlow::eRender
    DWORD mask) { // = DEVICE_STATE_ACTIVE
    
    IMMDeviceCollection* ptr_IMMDeviceCollection;
    HRESULT hr = this->ptr_instance->EnumAudioEndpoints(
        data_flow,
        mask,
        &ptr_IMMDeviceCollection
    );
    if (FAILED(hr))
        Cynth::Logger::errHRESULT(hr);
    Cynth::WASAPI::DeviceCollection device_collection(ptr_IMMDeviceCollection);
    device_collection.noAutoRelease();
    return device_collection;
}

Cynth::WASAPI::Device Cynth::WASAPI::DeviceEnumerator::getDefaultAudioEndpoint(
    EDataFlow data_flow, // = EDataFlow::eRender
    ERole role) { // = ERole::eMultimedia
    
    IMMDevice* ptr_IMMDevice;
    HRESULT hr = this->ptr_instance->GetDefaultAudioEndpoint(
        data_flow,
        role,
        &ptr_IMMDevice);
    if (FAILED(hr))
        Cynth::Logger::errHRESULT(hr);
    Cynth::WASAPI::Device device(ptr_IMMDevice);
    device.noAutoRelease();
    return device;
}

/* -- Device Collection: ---------------------------------------------------- */

int Cynth::WASAPI::DeviceCollection::getCount() {
    UINT device_count;
    HRESULT hr = this->ptr_instance->GetCount(&device_count);
    if (FAILED(hr))
        Cynth::Logger::errHRESULT(hr);
    return (int) device_count;
}

Cynth::WASAPI::Device Cynth::WASAPI::DeviceCollection::item(UINT i) {
    IMMDevice* ptr_IMMDevice;
    HRESULT hr = this->ptr_instance->Item(i, &ptr_IMMDevice);
    if (FAILED(hr))
        Cynth::Logger::errHRESULT(hr);
    Cynth::WASAPI::Device device(ptr_IMMDevice);
    device.noAutoRelease();
    return device;
}

/* -- Device: --------------------------------------------------------------- */

Cynth::WASAPI::Device::Device(IMMDevice* ptr_instance) // = nullptr
    : Cynth::WASAPI::Interface<IMMDevice>(ptr_instance) {}

void Cynth::WASAPI::Device::setup() {
    // TODO: Exceptions not working.
    if (! this->ptr_instance)
        Cynth::Logger::errWASAPI(
            "Pointer to COM interface not initialized "
            "before activating audio client.");
    this->audio_client = this->getAudioClient();
    this->audio_client.autoRelease();
    this->getMixFormat();
    /*WAVEFORMATEXTENSIBLE* ptr_wave_format_extensible
        = (WAVEFORMATEXTENSIBLE*) this->wave_format;
    std::string sub_format;
    sub_format
        = Cynth::Tools::guidToString(ptr_wave_format_extensible->SubFormat);
    Cynth::Logger::log(sub_format);
    Cynth::Logger::errCynth("STOP");*/
    this->checkFormatSupport();
    this->getDevicePeriod();
    this->initAudioClient();
    this->getBufferSize();
    this->audio_client.render_client = this->getRenderClient();
}

void Cynth::WASAPI::Device::getMixFormat() {
    WAVEFORMATEX* ptr_mix_format;
    HRESULT hr = this->audio_client->GetMixFormat(
        &ptr_mix_format);
    if (FAILED(hr))
        Cynth::Logger::errHRESULT(hr);
    this->wave_format = ptr_mix_format;
}

Cynth::WASAPI::AudioClient Cynth::WASAPI::Device::getAudioClient() {
    IID IID_IAudioClient = __uuidof(IAudioClient);
    IAudioClient* ptr_IAudioClient;
    HRESULT hr = this->ptr_instance->Activate(
        IID_IAudioClient,
        CLSCTX_ALL,
        NULL,
        (void**) &ptr_IAudioClient);
    if (FAILED(hr))
        Cynth::Logger::errHRESULT(hr);
    AudioClient audio_client(ptr_IAudioClient);
    audio_client.noAutoRelease();
    return audio_client;
}

void Cynth::WASAPI::Device::initAudioClient() {
    HRESULT hr = this->audio_client->Initialize(
        AUDCLNT_SHAREMODE_SHARED,
        0, // TODO: Check out the flags. // Previously: AUDCLNT_STREAMFLAGS_EVENTCALLBACK
        this->default_device_period, // Previously: minimum_device_period
        0,
        this->wave_format,
        NULL); // TODO: How to generate session GUID?
    if (FAILED(hr))
        Cynth::Logger::errHRESULT(hr);
}

Cynth::WASAPI::RenderClient Cynth::WASAPI::Device::getRenderClient() {
    const IID IID_IAudioRenderClient = __uuidof(IAudioRenderClient);
    IAudioRenderClient* ptr_IAudioRenderClient;
    HRESULT hr = this->audio_client->GetService(
        IID_IAudioRenderClient,
        (void**) &ptr_IAudioRenderClient);
    if (FAILED(hr))
        Cynth::Logger::errHRESULT(hr);
    Cynth::WASAPI::RenderClient render_client(ptr_IAudioRenderClient);
    render_client.noAutoRelease();
    return render_client;
}

void Cynth::WASAPI::Device::releaseAudioClient() {
    // TODO: Better abstraction.
    this->audio_client.render_client->Release();
    this->audio_client->Release();
    this->audio_client = nullptr;
}

void Cynth::WASAPI::Device::getDevicePeriod() {
    HRESULT hr = this->audio_client->GetDevicePeriod(
        &this->default_device_period,
        &this->minimum_device_period);
    if (FAILED(hr))
        Cynth::Logger::errHRESULT(hr);
}

void Cynth::WASAPI::Device::getBufferSize() {
    UINT32 buffer_size;
    HRESULT hr = this->audio_client->GetBufferSize(&buffer_size);
    if (FAILED(hr))
        Cynth::Logger::errHRESULT(hr);
    this->buffer_size_frames = buffer_size;
    this->buffer_size_bytes =
        this->buffer_size_frames
        * this->wave_format->nChannels
        * (this->wave_format->wBitsPerSample / 8);
}

void Cynth::WASAPI::Device::checkFormatSupport() {
    if (! this->audio_client.ptr_instance)
        Cynth::Logger::errWASAPI(
            "Pointer to COM interface not initialized "
            "before checking format support.");
    WAVEFORMATEX* ptr_wave_format = this->wave_format;
    WAVEFORMATEX* ptr_closest_format = nullptr;
    HRESULT hr = this->audio_client->IsFormatSupported(
        AUDCLNT_SHAREMODE_SHARED,
        ptr_wave_format,
        &ptr_closest_format);
    // TODO: if (hr == AUDCLNT_E_UNSUPPORTED_FORMAT) {}
    if (FAILED(hr)) {
        Cynth::Logger::errHRESULT(hr);
    }
    if (ptr_closest_format) {
        this->wave_format = ptr_closest_format;
    }
}

void Cynth::WASAPI::Device::setName(std::string name) {
    this->name = name;
}

Cynth::WASAPI::PropertyStore
Cynth::WASAPI::Device::openPropertyStore(DWORD access) { // = STGM_READ
    IPropertyStore* ptr_IPropertyStore;
    HRESULT hr = this->ptr_instance->OpenPropertyStore(access, &ptr_IPropertyStore);
    if (FAILED(hr))
        Cynth::Logger::errHRESULT(hr);
    Cynth::WASAPI::PropertyStore property_store(ptr_IPropertyStore);
    property_store.noAutoRelease();
    return property_store;
}

std::string Cynth::WASAPI::Device::getId() {
    LPWSTR id_wchar;
    this->ptr_instance->GetId(&id_wchar);
    std::string id_str;
    id_str = Cynth::Tools::wcharToString(id_wchar);
    return id_str;
}

/* -- Property Store: ------------------------------------------------------- */

Cynth::WASAPI::PropertyStore::~PropertyStore() {
    PropVariantClear(&this->property.prop_variant);
}

Cynth::WASAPI::PropertyStore::PropertyStore()
    : Cynth::WASAPI::Interface<IPropertyStore>::Interface() {}

Cynth::WASAPI::PropertyStore::PropertyStore(IPropertyStore* ptr_instance)
    : Cynth::WASAPI::Interface<IPropertyStore>::Interface(ptr_instance) {}

Cynth::WASAPI::PropertyStore::PropertyStore(
    const Cynth::WASAPI::PropertyStore& other)
    : Cynth::WASAPI::Interface<IPropertyStore>::Interface(other) {}

Cynth::WASAPI::PropertyStore::PropertyStore(
    Cynth::WASAPI::PropertyStore&& other)
    = default;

Cynth::WASAPI::PropertyStore&
Cynth::WASAPI::PropertyStore::operator=(
    const Cynth::WASAPI::PropertyStore& other) {
    
    Cynth::WASAPI::Interface<IPropertyStore>::operator=(other);
    return *this;
}

Cynth::WASAPI::PropertyStore&
Cynth::WASAPI::PropertyStore::operator=(
    Cynth::WASAPI::PropertyStore&& other)
    = default;

Cynth::WASAPI::PropertyStore::prop_t&
Cynth::WASAPI::PropertyStore::getValue(PROPERTYKEY property_key) {
    PropVariantClear(&this->property.prop_variant);
    PropVariantInit(&this->property.prop_variant);
    HRESULT hr = this->ptr_instance->GetValue(
        property_key,
        &this->property.prop_variant);
    if (FAILED(hr))
        Cynth::Logger::errHRESULT(hr);
    this->property.str_val
        = Cynth::Tools::wcharToString(this->property.prop_variant.pwszVal);
    return this->property;
}

/* -- Audio Client: --------------------------------------------------------- */



/* -- WASAPI Control: ------------------------------------------------------- */

Cynth::WASAPI::Control::Control() {
    // Initialize COM library:
    CoInitialize(NULL);
    // Fix missing PKEYs:
    this->fixMissingPKEYs();
    // COM result:
    HRESULT hr;
    // IMMDeviceEnumerator:
    DeviceEnumerator device_enumerator;
    // IMMDeviceCollection:
    DeviceCollection device_collection;
    device_collection = device_enumerator.getDeviceCollection();
    device_collection.autoRelease();
    // Device count:
    int device_count = device_collection.getCount();

    Device default_device;
    default_device = device_enumerator.getDefaultAudioEndpoint();
    std::string default_id = default_device.getId();
    default_device->Release();

    /* Load devices: */
    for (int i = 0; i < device_count; i++) {
        // Device interface instance:
        Cynth::WASAPI::Device device;
        device = device_collection.item(i);
        // Property store interface instance:
        Cynth::WASAPI::PropertyStore property_store
            = device.openPropertyStore();
        property_store.autoRelease();
        // Device property:
        device.name = property_store.getValue(
            PKEY_Device_FriendlyName).str_val;
        // Save the device:
        this->rendering_devices.push_back(std::move(device));
        this->rendering_devices.back().autoRelease();
        // Set if default:
        if (device.getId() == default_id)
            this->setRenderingDevice(i);
    }

/* -- Temporary testing: ---------------------------------------------------- */

    Cynth::WASAPI::Device& active_rendering_device
        = this->rendering_devices[this->active_rendering_devices[0]];

	float freq
        = (float) active_rendering_device.wave_format->nSamplesPerSec
        / (float) active_rendering_device.buffer_size_frames;
    std::cout << "freq: " << freq << std::endl;
    
    int buffer_period_ms
        = active_rendering_device.default_device_period / 10000;
    Cynth::Logger::log(buffer_period_ms);

    UINT32 buffer_size_samples = 2 * active_rendering_device.buffer_size_frames;
    int j = 0;
    bool first = true;
    BYTE* ptr_buffer;
    DWORD flags = 0;
    std::vector<unsigned long> samples(buffer_size_samples);
    int bit_depth = active_rendering_device.wave_format->wBitsPerSample;
    Sample sample(bit_depth);
    Cynth::Buffer buffer(bit_depth);
	while (true) {
		std::this_thread::sleep_for(
            std::chrono::milliseconds(buffer_period_ms));

        UINT32 padding = 0;
        hr = active_rendering_device.audio_client->GetCurrentPadding(&padding);
        if (FAILED(hr))
            Cynth::Logger::errHRESULT(hr);

		hr = active_rendering_device.audio_client.render_client->GetBuffer(
            active_rendering_device.buffer_size_frames - padding, &ptr_buffer);
		if (FAILED(hr))
            Cynth::Logger::errHRESULT(hr);

        unsigned long buffer_size_frames
            = active_rendering_device.buffer_size_frames - padding;
        for (unsigned long i = 0; i < buffer_size_frames; i++) {
            float t
                = (float) 2 * (i + j * buffer_size_frames)
                / active_rendering_device.wave_format->nSamplesPerSec;
            long max = (1 << (bit_depth - 1)) - 1;
            sample = Cynth::sin_float(400, t);
            //float* ptr_sample = (float*) sample.data();
            //std::cout << *ptr_sample << std::endl;
            /*sample = Cynth::sin_tmp(
                std::numeric_limits<long>::max(), 40, t);*/
            //sample = (long) (std::pow(2, bit_depth - 1) - 1) / 2;
            //sample = (long) 0x40000000;
            buffer.write(sample); // i   Left channel
            buffer.write(sample); // i+1 Right channel
        }
        j++;

        //buffer.clear();
        buffer.moveTo(ptr_buffer);
        /*float* ptr_sample = (float*) ptr_buffer;
        for (unsigned long i = 0; i < buffer_size_frames; i++) {
            std::cout << *ptr_sample << std::endl;
            std::cout << *ptr_sample << std::endl;
            ptr_sample++;
        }*/

		hr = active_rendering_device.audio_client.render_client->ReleaseBuffer(
            active_rendering_device.buffer_size_frames - padding, flags);
		if (FAILED(hr))
            Cynth::Logger::errHRESULT(hr);

        if (first) {
            hr = active_rendering_device.audio_client->Start();
            if (FAILED(hr))
                Cynth::Logger::errHRESULT(hr);
            first = false;
        }
	}

    std::this_thread::sleep_for(
            std::chrono::milliseconds(buffer_period_ms));

	hr = active_rendering_device.audio_client->Stop();  // Stop playing.
	if (FAILED(hr))
        Cynth::Logger::errHRESULT(hr);
/* -------------------------------------------------------------------------- */
}

std::vector<std::string>
Cynth::WASAPI::Control::getRenderingDevicesNames() {
    std::vector<std::string> names;
    for(int i = 0; i < (int) this->rendering_devices.size(); i++) {
        names.push_back(this->rendering_devices[i].name);
    }
    return names;
}

int Cynth::WASAPI::Control::getRenderingDevicesCount() {
    return this->rendering_devices.size();
}

void Cynth::WASAPI::Control::clearRenderingDevices() {
    for (int i = 0; i < (int) this->active_rendering_devices.size(); i++)
        this->rendering_devices[this->active_rendering_devices[i]]
            .releaseAudioClient();
    this->active_rendering_devices.clear();
}

void Cynth::WASAPI::Control::setRenderingDevices(std::vector<int> ids) {
    this->clearRenderingDevices();
    this->active_rendering_devices = ids;
    for (int i = 0; i < (int) ids.size(); i++)
        this->rendering_devices[ids[i]].setup();
}

void Cynth::WASAPI::Control::setRenderingDevice(int id) {
    this->clearRenderingDevices();
    std::vector<int> ids = {id};
    this->active_rendering_devices = ids;
    this->rendering_devices[id].setup();
}

void Cynth::WASAPI::Control::addRenderingDevice(int id) {
    this->active_rendering_devices.push_back(id);
    this->rendering_devices[id].setup();
}