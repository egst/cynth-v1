#pragma once

/* Local libraries: */
#include "rack/devices/oscillator.hpp"
#include "pcm/functions/wave_function.hpp"

namespace Cynth::UserLibrary::Devices {

    /*/ Tone Generator Device:

        Used to generade audible waves.
    /*/
    class ToneGenerator: public Cynth::Rack::Devices::Oscillator<
        Cynth::PCM::Functions::WaveFunction> {
    public:
        /* Constructors: */
        // With custom wave function:
        ToneGenerator(
            WaveFunction wave_f,
            float freq = 440,
            float amp = 0,
            freq_type_t freq_type = freq_type_t::HZ);
        // With basic wave function:
        ToneGenerator(
            wave_func_t wave_f,
            float freq = 440,
            float amp = 0,
            freq_type_t freq_type = freq_type_t::HZ);
    };
    
}