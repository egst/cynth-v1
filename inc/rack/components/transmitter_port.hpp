#pragma once

/* Local libraries: */
#include "user_library/functions/wave_function.hpp"

/* Standard libraries: */
#include <functional>

namespace Cynth::Rack::Components {

    class TransmitterPort {
    private:
        /* Aliases: */
        using WaveFunction = Cynth::UserLibrary::Functions::WaveFunction;

        WaveFunction* ptr_wave_function;
    public:
        /* Constructor: */
        TransmitterPort();

        /* Mutators: */
        TransmitterPort& operator<<(WaveFunction& f);

        /* Accessors: */
        float operator()(double offset);
    };

}