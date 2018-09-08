#pragma once

/* Local libraries: */
#include "rack/components/receiver_port.hpp"
#include "rack/components/transmitter_port.hpp"
#include "rack/devices/input_device.hpp"
#include "user_library/functions/wave_function.hpp"
#include "user_library/devices/types.hpp"

/* Standard libraries: */
#include <vector>

namespace Cynth::UserLibrary::Devices {

    class Hub: public Rack::Devices::InputDevice {
    private:
        /* Aliases: */
        using ReceiverPort = Cynth::Rack::Components::ReceiverPort;
        using TransmitterPort = Cynth::Rack::Components::TransmitterPort;
        using WaveFunction = Cynth::UserLibrary::Functions::WaveFunction;

        std::vector<ReceiverPort> input_ports;

        WaveFunction comp_function;

    public:
        Hub(comp_type_t comp_type = comp_type_t::ADD);

        Hub& operator<<(TransmitterPort& port);
    };
}