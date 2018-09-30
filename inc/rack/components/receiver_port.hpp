#pragma once

/* Local libraries: */
#include "platform.hpp"
#include "rack/components/transmitter_port.hpp"

/* Standard libraries: */
#include <functional>

namespace Cynth::Rack::Components {

    class ReceiverPort {
    private:
        /* Aliases: */
        using TransmitterPort = Rack::Components::TransmitterPort;

        const TransmitterPort* ptr_transmitter_port;
    public:
        /* Constructor: */
        ReceiverPort();

        /* Mutators: */
        CYNTH_EXPORT ReceiverPort& operator<<(const TransmitterPort& port);

        /* Accessors: */
        float operator()(double offset) const;
    };

}