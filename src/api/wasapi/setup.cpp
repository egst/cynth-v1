/* An implementation file for: */
#include "api/wasapi/setup.hpp"

/* Local libraries: */
#include "logger.hpp"
#include "api/wasapi/device_enumerator.hpp"
#include "api/wasapi/device_collection.hpp"
#include "api/wasapi/device.hpp"
#include "pcm/sample.hpp"                   // temporary testing
#include "pcm/buffer.hpp"                   // temporary testing
#include "user_library/functions/wave_functions.hpp" // temporary testing

/* Standard libraries: */
#include <iostream> // temporary testing
#include <chrono>   // temporary testing
#include <thread>   // temporary testing
#include <cmath>    // temporary testing

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

    /* -- Temporary testing: ---------------------------------------------------- */
    #if 0

    HRESULT hr;

    Device& active_rendering_device
        = this->active_rendering_devices.front().get();
    
    int buffer_period_ms
        = active_rendering_device.default_device_period / 10000;

    UINT32 buffer_size_samples = 2 * active_rendering_device.buffer_size_frames;
    int j = 0;
    bool first = true;
    BYTE* ptr_buffer;
    DWORD flags = 0;
    int bit_depth = active_rendering_device.ptr_wave_format->wBitsPerSample;
    Cynth::PCM::Sample sample(bit_depth);
    Cynth::PCM::Buffer buffer(bit_depth);
    Cynth::PCM::Functions::WaveFs wave_fs;
	while (true) {
		std::this_thread::sleep_for(
            std::chrono::milliseconds(buffer_period_ms / 2));

        UINT32 padding = 0;
        hr = active_rendering_device.audio_client->GetCurrentPadding(&padding);
        if (FAILED(hr))
            Logger::errHRESULT(hr);

		hr = active_rendering_device.audio_client.render_client->GetBuffer(
            active_rendering_device.buffer_size_frames - padding, &ptr_buffer);
		if (FAILED(hr))
            Logger::errHRESULT(hr);

        unsigned long buffer_size_frames
            = active_rendering_device.buffer_size_frames - padding;
        for (unsigned long i = 0; i < buffer_size_frames; i++) {
            float t
                = (float) 2 * (i + j * buffer_size_frames)
                / active_rendering_device.ptr_wave_format->nSamplesPerSec;
            //std::cout << t << std::endl;
            sample = (float) 0.1 * wave_fs.sine(400 * t);
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
            Logger::errHRESULT(hr);

        if (first) {
            hr = active_rendering_device.audio_client->Start();
            if (FAILED(hr))
                Logger::errHRESULT(hr);
            first = false;
        }
	}

    std::this_thread::sleep_for(
            std::chrono::milliseconds(buffer_period_ms));

	hr = active_rendering_device.audio_client->Stop();  // Stop playing.
	if (FAILED(hr))
        Logger::errHRESULT(hr);
    #endif
    /* ---------------------------------------------------------------------- */

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