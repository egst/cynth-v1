/* An implementation file for: */
#include "pcm/functions/wave_functions.hpp"

/* Local libraries: */
#include "tools.hpp"

/* Standard libraries: */
#define _USE_MATH_DEFINES // -> M_PI
#include <cmath>

using namespace Cynth::PCM::Functions;
using Cynth::Tools::compare;

WaveFs::WaveFs() {

    this->sine = [](float offset) -> float {
        //offset = std::fmod(offset, 1);
        return std::sin(2 * M_PI * offset);
    };

    this->square = [](float offset) -> float {
        offset = std::fmod(offset, 1);
        if (compare(offset, 0)
        ||  compare(offset, 0.5)
        ||  compare(offset, 1))
            return 0;
        else if (offset < 0.5)
            return 1;
        else
            return -1;
    };

    this->saw = [](float offset) -> float {
        offset = std::fmod(offset, 1);
        if (compare(offset, 0.5))
            return 0;
        else if (offset < 0.5)
            return 2 * offset;
        else
            return 2 * offset - 2;
    };

    this->triangle = [](float offset) -> float {
        offset = std::fmod(offset, 1);
        if (offset < 0.25)
            return 4 * offset;
        if (compare(offset, 0.25))
            return 1;
        else if (offset < 0.75)
            return -4 * offset + 2;
        if (compare(offset, 0.75))
            return -1;
        else
            return 4 * offset - 4;
    };
}