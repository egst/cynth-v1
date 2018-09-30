#pragma once

/* Local libraries: */
#include "rack/components/transmitter_port.hpp"
#include "rack/components/receiver_port.hpp"

namespace Cynth::Rack::Devices {

    class OutputDevice {
    protected:
        /* Aliases: */
        using TransmitterPort = Cynth::Rack::Components::TransmitterPort;
        using ReceiverPort = Cynth::Rack::Components::ReceiverPort;
    public:
        OutputDevice();

        /* Connect input: */
        CYNTH_EXPORT OutputDevice& operator<<(TransmitterPort& port);

        ReceiverPort input_port;
    };

}