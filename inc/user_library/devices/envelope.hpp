#pragma once

/* Local libraries: */
#include "platform.hpp"
#include "rack/devices/input_device.hpp"
#include "rack/devices/output_device.hpp"
#include "user_library/functions/wave_function.hpp"

namespace Cynth::UserLibrary::Devices {

    class CYNTH_EXPORT Envelope:
        public Cynth::Rack::Devices::InputDevice,
        public Cynth::Rack::Devices::OutputDevice {
        
    private:
        /* Aliases: */
        using WaveFunction = Cynth::UserLibrary::Functions::WaveFunction;

        WaveFunction env_function;
        WaveFunction mod_function;

        double attack_time;
        double decay_time;
        double sustain_level;
        double release_time;

    public:
        Envelope();
    };
}