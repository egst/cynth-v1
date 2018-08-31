/* An implementation file for: */
#include "api/wasapi/device_collection.hpp"

/* Local libraries: */
#include "logger.hpp"

using namespace Cynth::API::WASAPI;
using Cynth::Logger;

int DeviceCollection::getCount() {
    UINT device_count;
    HRESULT hr = this->ptr_instance->GetCount(&device_count);
    if (FAILED(hr))
        Logger::errHRESULT(hr);
    return (int) device_count;
}

Device DeviceCollection::item(UINT i) {
    IMMDevice* ptr_IMMDevice;
    HRESULT hr = this->ptr_instance->Item(i, &ptr_IMMDevice);
    if (FAILED(hr))
        Logger::errHRESULT(hr);
    Device device(ptr_IMMDevice);
    device.noAutoRelease();
    return device;
}