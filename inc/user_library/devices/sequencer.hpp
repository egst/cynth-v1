#pragma once

/* Local libraries: */
#include "rack/devices/oscillator.hpp"
#include "pcm/functions/sequence_function.hpp"

namespace Cynth::UserLibrary::Devices {

    /*/ Sequencer Device:

        Used to generate rhythmic patters of frequency values.
    /*/
    class Sequencer: public Cynth::Rack::Devices::Oscillator {
    public:
        /* Constructors: */
        // With custom sequence function:
        Sequencer(
            WaveFunction f,
            float freq = 1,
            float amp = 0,
            freq_type_t freq_type = freq_type_t::MF);
        // TODO: With basic sequence function?
    };

}