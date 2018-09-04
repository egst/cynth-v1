#pragma once

/* Local libraries: */
#include "pcm/functions/wave_function.hpp"

/* Standard libraries: */
#include <map>
#include <string>

namespace Cynth::PCM::Functions {

    /*/ Basic wave functions: /*/
    enum wave_func_t {
        SINE,
        SQUARE,
        SAW,
        TRIANGLE
    };

    class WaveFs {
    public:
        /* Constructor: */
        // Initializes the functions.
        WaveFs();

        /* Static Functions: */
        // Sine Wave:
        WaveFunction sine;
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