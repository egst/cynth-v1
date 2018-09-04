#pragma once

/* Local libraries: */
#include "api/wasapi/device.hpp"

/* Standard libraries: */
#include <vector>
#include <functional>

namespace Cynth::API::WASAPI {

    class Setup {
    private:
        /* Properties: */
        // Rendering devices:
        std::vector<Device> rendering_devices;
        // Active rendering devices references:
        std::vector<std::reference_wrapper<Device>> active_rendering_devices;

        /* Mutators: */
        void clearRenderingDevices();

    public:
        /* Constructor:
        
        Initializes COM library.
        Gets all available rendering devices from IMMDeviceEnumerator
        and saves them in rendering_devices vector.
        Saves the default rendering devices in active_rendering_devices vector.
        */
        Setup();

        /* Fix for missing COM constants: */
        void fixMissingPKEYs();

        /* Accessors: */
        std::vector<std::reference_wrapper<Device>>& getActiveRenderingDevices();
        std::vector<std::string> getRenderingDevicesNames();
        int getRenderingDevicesCount();

        /* Mutators: */
        void setRenderingDevices(std::vector<int> ids);
        void setRenderingDevice(int id);
        void addRenderingDevice(int id);
    };

}