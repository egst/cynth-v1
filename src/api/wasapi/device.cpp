/* An implementation file for: */
#include "api/wasapi/device.hpp"

/* Local libraries: */
#include "logger.hpp"
#include "tools.hpp"

using namespace Cynth::API::WASAPI;
using Cynth::Logger;

Device::Device(IMMDevice* ptr_instance) // = nullptr
    : Interface<IMMDevice>(ptr_instance) {}

void Device::setup() {
    if (! this->ptr_instance)
        Logger::errWASAPI(
            "Pointer to COM interface not initialized "
            "before activating audio client.");
    this->audio_client = this->getAudioClient();
    this->audio_client.autoRelease();
    this->getMixFormat();
    // TODO: Get amplitude format.
    /*WAVEFORMATEXTENSIBLE* ptr_wave_format_extensible
        = (WAVEFORMATEXTENSIBLE*) this->wave_format;
    std::string sub_format;
    sub_format
        = Cynth::Tools::guidToString(ptr_wave_format_extensible->SubFormat);
    Logger::log(sub_format);
    Logger::errCynth("STOP");*/
    this->checkFormatSupport();
    this->getDevicePeriod();
    this->initAudioClient();
    this->getBufferSize();
    this->audio_client.render_client = this->getRenderClient();
}

void Device::getMixFormat() {
    WAVEFORMATEX* ptr_mix_format;
    HRESULT hr = this->audio_client->GetMixFormat(
        &ptr_mix_format);
    if (FAILED(hr))
        Logger::errHRESULT(hr);
    this->wave_format = ptr_mix_format;
}

AudioClient Device::getAudioClient() {
    // TODO: Some errors here.
    IID IID_IAudioClient = __uuidof(IAudioClient);
    IAudioClient* ptr_IAudioClient;
    HRESULT hr = this->ptr_instance->Activate(
        IID_IAudioClient,
        CLSCTX_ALL,
        NULL,
        (void**) &ptr_IAudioClient);
    if (FAILED(hr))
        Logger::errHRESULT(hr);
    AudioClient audio_client(ptr_IAudioClient);
    audio_client.noAutoRelease();
    return audio_client;
}

void Device::initAudioClient() {
    HRESULT hr = this->audio_client->Initialize(
        AUDCLNT_SHAREMODE_SHARED,
        0, // TODO: Check out the flags. // Previously: AUDCLNT_STREAMFLAGS_EVENTCALLBACK
        this->default_device_period, // Previously: minimum_device_period
        0,
        this->wave_format,
        NULL); // TODO: How to generate session GUID?
    if (FAILED(hr))
        Logger::errHRESULT(hr);
}

RenderClient Device::getRenderClient() {
    // TODO: Some errors here.
    const IID IID_IAudioRenderClient = __uuidof(IAudioRenderClient);
    IAudioRenderClient* ptr_IAudioRenderClient;
    HRESULT hr = this->audio_client->GetService(
        IID_IAudioRenderClient,
        (void**) &ptr_IAudioRenderClient);
    if (FAILED(hr))
        Logger::errHRESULT(hr);
    RenderClient render_client(ptr_IAudioRenderClient);
    render_client.noAutoRelease();
    return render_client;
}

void Device::releaseAudioClient() {
    // TODO: Better abstraction.
    this->audio_client.render_client->Release();
    this->audio_client->Release();
    this->audio_client = nullptr;
}

void Device::getDevicePeriod() {
    HRESULT hr = this->audio_client->GetDevicePeriod(
        &this->default_device_period,
        &this->minimum_device_period);
    if (FAILED(hr))
        Logger::errHRESULT(hr);
}

void Device::getBufferSize() {
    UINT32 buffer_size;
    HRESULT hr = this->audio_client->GetBufferSize(&buffer_size);
    if (FAILED(hr))
        Logger::errHRESULT(hr);
    this->buffer_size_frames = buffer_size;
    this->buffer_size_bytes =
        this->buffer_size_frames
        * this->wave_format->nChannels
        * (this->wave_format->wBitsPerSample / 8);
}

void Device::checkFormatSupport() {
    if (! this->audio_client.ptr_instance)
        Logger::errWASAPI(
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
        Logger::errHRESULT(hr);
    }
    if (ptr_closest_format) {
        this->wave_format = ptr_closest_format;
    }
}

void Device::setName(std::string name) {
    this->name = name;
}

PropertyStore Device::openPropertyStore(DWORD access) { // = STGM_READ
    IPropertyStore* ptr_IPropertyStore;
    HRESULT hr = this->ptr_instance->OpenPropertyStore(access, &ptr_IPropertyStore);
    if (FAILED(hr))
        Logger::errHRESULT(hr);
    PropertyStore property_store(ptr_IPropertyStore);
    property_store.noAutoRelease();
    return property_store;
}

std::string Device::getId() {
    LPWSTR id_wchar;
    this->ptr_instance->GetId(&id_wchar);
    std::string id_str;
    id_str = Cynth::Tools::wcharToString(id_wchar);
    return id_str;
}