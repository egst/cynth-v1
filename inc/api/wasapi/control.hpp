#pragma once

/* Local libraries: */
#include "api/wasapi/device.hpp"

/* Standard libraries: */
#include <vector>

namespace Cynth::API::WASAPI {

    class Control {
    private:
        /* Properties: */
        // Rendering devices:
        std::vector<Device> rendering_devices;
        // Active rendering devices ids in the rendering_devices vector:
        // Any number of devices may be chosen,
        // each receives audio stream as a single channel.
        std::vector<int> active_rendering_devices;

        /* Mutators: */
        void clearRenderingDevices();

    public:
        /* Constructor:
        
        Initializes COM library.
        Gets all available rendering devices from IMMDeviceEnumerator
        and saves them in rendering_devices vector.
        Saves the default rendering device in active_rendering_devices
        vector. */
        Control();

        /* Fix missing PKEYs: */
        void fixMissingPKEYs();

        /* Accessors: */
        // Get available devices:
        std::vector<std::string> getRenderingDevicesNames();
        int getRenderingDevicesCount();

        /* Mutators: */
        // Set active rendering devices:
        void setRenderingDevices(std::vector<int> ids);
        void setRenderingDevice(int id);
        void addRenderingDevice(int id);
    };

}