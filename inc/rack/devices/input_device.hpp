#pragma once

/* Local libraries: */
#include "rack/components/transmitter_port.hpp"

namespace Cynth::Rack::Devices {

    class InputDevice {
    protected:
        /* Aliases: */
        using TransmitterPort = Cynth::Rack::Components::TransmitterPort;
    public:
        TransmitterPort output_port;
        
        InputDevice();
    };

}