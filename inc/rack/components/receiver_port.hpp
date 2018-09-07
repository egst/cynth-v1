#pragma once

/* Local libraries: */
#include "rack/components/transmitter_port.hpp"

/* Standard libraries: */
#include <functional>

namespace Cynth::Rack::Components {

    class ReceiverPort {
    private:
        /* Aliases: */
        using TransmitterPort = Rack::Components::TransmitterPort;

        TransmitterPort* ptr_transmitter_port;
    public:
        /* Constructor: */
        ReceiverPort();

        /* Mutators: */
        ReceiverPort& operator<<(TransmitterPort& port);

        /* Accessors: */
        float operator()(double offset);
    };

}