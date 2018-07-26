/* An implementation file for: */
#include "WASAPI.hpp"

/* Local libraries: */
#include "tools.hpp"
#include "logger.hpp"

/* Windows libraries: */
// For more info see doc/WASAPI_notes.md
// Audio Client:
#include <audioclient.h>
// Property System:
//#include <propsys.h> // TOCH: Needed?

#include <iostream> //tmp

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
Cynth::WASAPI::Interface<interface_t>::Interface(): auto_release(true) {}

template<typename interface_t>
Cynth::WASAPI::Interface<interface_t>::Interface(interface_t* ptr_instance)
    : ptr_instance(ptr_instance), auto_release(true) {}

template<typename interface_t>
Cynth::WASAPI::Interface<interface_t>::~Interface() {
    if (this->auto_release)
        this->ptr_instance->Release();
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
        Cynth::Logger::hresultErr(hr);
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
        Cynth::Logger::hresultErr(hr);
    Cynth::WASAPI::DeviceCollection device_collection(ptr_IMMDeviceCollection);
    device_collection.noAutoRelease();
    return device_collection;
}

/* -- Device Collection: ---------------------------------------------------- */

UINT Cynth::WASAPI::DeviceCollection::getCount() {
    UINT device_count;
    HRESULT hr = this->ptr_instance->GetCount(&device_count);
    if (FAILED(hr))
        Cynth::Logger::hresultErr(hr);
    return device_count;
}

Cynth::WASAPI::Device Cynth::WASAPI::DeviceCollection::item(UINT i) {
    IMMDevice* ptr_IMMDevice;
    HRESULT hr = this->ptr_instance->Item(i, &ptr_IMMDevice);
    if (FAILED(hr))
        Cynth::Logger::hresultErr(hr);
    Cynth::WASAPI::Device device(ptr_IMMDevice);
    device.noAutoRelease();
    return device;
}

/* -- Device: --------------------------------------------------------------- */

void Cynth::WASAPI::Device::setName(std::string name) {
    this->name = name;
}

Cynth::WASAPI::PropertyStore
Cynth::WASAPI::Device::openPropertyStore(DWORD access) { // = STGM_READ
    IPropertyStore* ptr_IPropertyStore;
    HRESULT hr = this->ptr_instance->OpenPropertyStore(STGM_READ, &ptr_IPropertyStore);
    if (FAILED(hr))
        Cynth::Logger::hresultErr(hr);
    Cynth::WASAPI::PropertyStore property_store(ptr_IPropertyStore);
    property_store.noAutoRelease();
    return property_store;
}

/* -- Property Store: ------------------------------------------------------- */

Cynth::WASAPI::PropertyStore::~PropertyStore() {
    PropVariantClear(&this->property.prop_variant);
}

Cynth::WASAPI::PropertyStore::prop_t&
Cynth::WASAPI::PropertyStore::getValue(PROPERTYKEY property_key) {
    PropVariantClear(&this->property.prop_variant);
    PropVariantInit(&this->property.prop_variant);
    HRESULT hr = this->ptr_instance->GetValue(
        property_key,
        &this->property.prop_variant);
    if (FAILED(hr))
        Cynth::Logger::hresultErr(hr);
    this->property.str_val
        = Cynth::Tools::wcharToString(this->property.prop_variant.pwszVal);
    return this->property;
}

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
    UINT device_count;
    device_count = device_collection.getCount();

    /* List devices: */
    for (UINT i = 0; i < device_count; i++) {
        /* Init: */
        // Device properties:
        std::string device_name;
        LPWSTR device_id;
        // Device interface instance:
        Cynth::WASAPI::Device device;
        device = device_collection.item(i);
        device.autoRelease();
        // Property store interface instance:
        Cynth::WASAPI::PropertyStore property_store;
        property_store = device.openPropertyStore();
        property_store.autoRelease();
        // Device property:
        device.name = property_store.getValue(
            PKEY_Device_FriendlyName).str_val;

        /* Save the device: */
        this->rendering_devices.push_back(device);
    }
}

std::vector<std::string>
Cynth::WASAPI::Control::getRenderingDevicesNames() {
    std::vector<std::string> names;
    for(int i = 0; i < this->rendering_devices.size(); i++) {
        names.push_back(this->rendering_devices[i].name);
    }
    return names;
}

void Cynth::WASAPI::Control::setRenderingDevices(std::vector<int> ids) {
    // ...
}