#pragma once

/* Local libraries: */
#include "platform.hpp"

/* Standard libraries: */
#include <vector>

namespace Cynth {

    class CLI {
    private:
        /* Properties: */
        // Audio API refference:
        CYNTH_API& audio_API;
        // Number of available rendering devices:
        int rendering_devices_count;

        /* Setup methods: */
        // Prints available rendering devices:
        void printRenderingDevices();
        // Sets the rendering devices:
        void setRenderingDevices();
        // Interactive prompt to choose rendering devices:
        std::vector<int> promptRenderingDevices();

    public:
        /* Constructor: */
        CLI(CYNTH_API audio_API);

        /* Setup: */
        void run();
    };

}