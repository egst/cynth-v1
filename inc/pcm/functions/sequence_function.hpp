#pragma once

/* Local libraries: */
#include "pcm/functions/time_function.hpp"

namespace Cynth::PCM::Functions {

    /*/ Sequence Function:
    
        Generates discrete values.
    /*/
    template<typename output_t>
    class SequenceFunction: public TimeFunction<output_t> {};

}