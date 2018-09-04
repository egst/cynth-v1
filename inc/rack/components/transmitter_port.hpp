#pragma once

/* Local libraries: */
#include "pcm/functions/wave_function.hpp"

/* Standard libraries: */
#include <functional>

namespace Cynth::Rack::Components {

    class TransmitterPort {
    private:
        /* Aliases: */
        using WaveFunction = Cynth::PCM::Functions::WaveFunction;

        WaveFunction* ptr_wave_function;
    public:
        /* Constructor: */
        TransmitterPort();

        /* Mutators: */
        TransmitterPort& operator<<(WaveFunction& f);

        /* Accessors: */
        float operator()(float offset);
    };

}