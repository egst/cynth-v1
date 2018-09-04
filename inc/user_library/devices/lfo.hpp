#pragma once

/* Local libraries: */
#include "rack/devices/oscillator.hpp"
#include "pcm/functions/wave_function.hpp"

namespace Cynth::UserLibrary::Devices {

    /*/ LFO Device:
        (Low Frequency Oscillator)

        Used to generade slower waves to modulate properties of other devices.
    /*/
    class LFO: public Cynth::Rack::Devices::Oscillator {
    public:
        /* Constructors: */
        // With custom wave function:
        LFO(
            WaveFunction wave_f,
            float freq = 1,
            float amp = 0,
            freq_type_t freq_type = freq_type_t::MF);
        // With basic wave function:
        LFO(
            wave_func_t wave_f,
            float freq = 1,
            float amp = 0,
            freq_type_t freq_type = freq_type_t::MF);
    };

}