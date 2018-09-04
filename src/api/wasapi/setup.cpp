/* An implementation file for: */
#include "api/wasapi/setup.hpp"

/* Local libraries: */
#include "logger.hpp"
#include "api/wasapi/device_enumerator.hpp"
#include "api/wasapi/device_collection.hpp"
#include "api/wasapi/device.hpp"
#include "pcm/sample.hpp"                   // temporary testing
#include "pcm/buffer.hpp"                   // temporary testing
#include "pcm/functions/wave_functions.hpp" // temporary testing

/* Standard libraries: */
#include <iostream> // temporary testing
#include <chrono>   // temporary testing
#include <thread>   // temporary testing

using namespace Cynth::API::WASAPI;
using Cynth::Logger;

PROPERTYKEY PKEY_Device_FriendlyName; // In global scope.
void Setup::fixMissingPKEYs() {
    GUID IDevice_FriendlyName = {
        0xa45c254e, 0xdf1c, 0x4efd,
        {0x80, 0x20, 0x67, 0xd1, 0x46, 0xa8, 0x50, 0xe0}};
    PKEY_Device_FriendlyName.pid = 14;
    PKEY_Device_FriendlyName.fmtid = IDevice_FriendlyName;
}

Setup::Setup() {
    // Initialize COM library:
    CoInitialize(NULL);
    // Fix missing PKEYs:
    this->fixMissingPKEYs();
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
        Device device;
        device = device_collection.item(i);
        // Property store interface instance:
        PropertyStore property_store = device.openPropertyStore();
        property_store.autoRelease();
        // Device property:
        device.name = property_store.getValue(PKEY_Device_FriendlyName).str_val;
        // Save the device:
        this->rendering_devices.push_back(std::move(device));
        this->rendering_devices.back().autoRelease();
        // Set if default:
        if (device.getId() == default_id)
            this->setRenderingDevice(i);
    }
}

std::vector<std::reference_wrapper<Device>>&
Setup::getActiveRenderingDevices() {
    return this->active_rendering_devices;
}

std::vector<std::string> Setup::getRenderingDevicesNames() {
    std::vector<std::string> names;
    for (auto& rendering_device : this->rendering_devices)
        names.push_back(rendering_device.name);
    return names;
}

int Setup::getRenderingDevicesCount() {
    return this->rendering_devices.size();
}

void Setup::clearRenderingDevices() {
    for (auto& active_rendering_device : this->active_rendering_devices)
        active_rendering_device.get().releaseAudioClient();
    this->active_rendering_devices.clear();
}

void Setup::setRenderingDevices(std::vector<int> ids) {
    this->clearRenderingDevices();
    for (auto& id : ids) {
        this->active_rendering_devices.push_back(std::ref(
            this->rendering_devices[id]));
        this->active_rendering_devices.back().get().setup();
    }
}

void Setup::setRenderingDevice(int id) {
    this->clearRenderingDevices();
    this->active_rendering_devices.push_back(std::ref(
        this->rendering_devices[id]));
    this->active_rendering_devices.back().get().setup();
}

void Setup::addRenderingDevice(int id) {
    this->active_rendering_devices.push_back(std::ref(
        this->rendering_devices[id]));
    this->active_rendering_devices.back().get().setup();
}