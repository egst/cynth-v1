#pragma once

#include "platform.hpp"

namespace Cynth {

    class Setup {
    private:
        /* Properties: */
        // Audio API refference:
        CYNTH_API& audio_API;

        //void printRenderingDevices();

    public:
        /* Constructor: */
        Setup(CYNTH_API audio_API);

        //void setRenderingDevices();
    };

}