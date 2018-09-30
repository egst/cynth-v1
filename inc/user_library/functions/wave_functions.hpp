#pragma once

/* Local libraries: */
#include "platform.hpp"
#include "user_library/functions/wave_function.hpp"

/* Standard libraries: */
#include <map>
#include <string>

namespace Cynth::UserLibrary::Functions {

    class CYNTH_EXPORT WaveFs {
    public:
        /* Constructor: */
        // Initializes the functions.
        WaveFs();

        /* Static Functions: */
        // TODO: Make private and allow only read access.
        // Sine Wave:
        WaveFunction sine;
        // Sine Wave Positive:
        WaveFunction sine_pos;
        // Square Wave:
        WaveFunction square;
        // Saw Wave:
        WaveFunction saw;
        // Triangle Wave:
        WaveFunction triangle;

        /* Custom functions container: */
        std::map<std::string, WaveFunction> custom;
    };

}