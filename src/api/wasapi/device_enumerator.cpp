/* An implementation file for: */
#include "api/wasapi/device_enumerator.hpp"

/* Local libraries: */
#include "logger.hpp"

/* Windows libraries: */
// Multimedia Device API:
#include <mmdeviceapi.h>

using namespace Cynth::API::WASAPI;
using Cynth::Logger;

DeviceEnumerator::DeviceEnumerator() {
    // TODO: Intellisense problems:
    #ifndef __INTELLISENSE__
    const CLSID CLSID_MMDeviceEnumerator = __uuidof(MMDeviceEnumerator);
    const IID IID_IMMDeviceEnumerator = __uuidof(IMMDeviceEnumerator);
    #endif
    HRESULT hr = CoCreateInstance(
        CLSID_MMDeviceEnumerator,
        NULL,
        CLSCTX_ALL,
        IID_IMMDeviceEnumerator,
        (void**) &this->ptr_instance);
    if (FAILED(hr))
        Logger::errHRESULT(hr);
}

DeviceCollection DeviceEnumerator::getDeviceCollection(
    EDataFlow data_flow, // = EDataFlow::eRender
    DWORD mask) { // = DEVICE_STATE_ACTIVE
    
    IMMDeviceCollection* ptr_IMMDeviceCollection;
    HRESULT hr = this->ptr_instance->EnumAudioEndpoints(
        data_flow,
        mask,
        &ptr_IMMDeviceCollection
    );
    if (FAILED(hr))
        Logger::errHRESULT(hr);
    DeviceCollection device_collection(ptr_IMMDeviceCollection);
    device_collection.noAutoRelease();
    return device_collection;
}

Device DeviceEnumerator::getDefaultAudioEndpoint(
    EDataFlow data_flow, // = EDataFlow::eRender
    ERole role) { // = ERole::eMultimedia
    
    IMMDevice* ptr_IMMDevice;
    HRESULT hr = this->ptr_instance->GetDefaultAudioEndpoint(
        data_flow,
        role,
        &ptr_IMMDevice);
    if (FAILED(hr))
        Logger::errHRESULT(hr);
    Device device(ptr_IMMDevice);
    device.noAutoRelease();
    return device;
}