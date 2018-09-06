#pragma once

/* Local libraries: */
#include "user_library/functions/wave_function.hpp"

/* Standard libraries: */
#include <map>
#include <string>

namespace Cynth::UserLibrary::Functions {

    class WaveFs {
    public:
        /* Constructor: */
        // Initializes the functions.
        WaveFs();

        /* Static Functions: */
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