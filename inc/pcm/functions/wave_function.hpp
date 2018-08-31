#pragma once

/* Local libraries: */
#include "pcm/functions/time_function.hpp"

namespace Cynth::PCM::Functions {

    /*/ Wave Function:
    
        Generates continuous values.
    /*/
    class WaveFunction: public TimeFunction<float> {
    protected:
        /* Type definitions: */
        // Function definition:
        using def_t = TimeFunction<float>::def_t;
    
    public:
        /* Constructor: */
        WaveFunction();
        WaveFunction(def_t def);

        using TimeFunction<float>::operator=;
    };

}