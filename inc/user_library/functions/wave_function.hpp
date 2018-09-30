#pragma once

/* Local libraries: */
#include "platform.hpp"
#include "pcm/functions/time_function.hpp"

namespace Cynth::UserLibrary::Functions {

    /*/ Wave Function:
    
        Generates continuous values.
    /*/
    class CYNTH_EXPORT WaveFunction: public Cynth::PCM::Functions::TimeFunction<float> {
    protected:
        /* Aliases: */
        template<typename output_t>
        using TimeFunction = Cynth::PCM::Functions::TimeFunction<output_t>;
    
    public:
        // Function definition:
        using def_t = TimeFunction<float>::def_t;

        /* Constructor: */
        WaveFunction();
        WaveFunction(def_t def);

        using TimeFunction<float>::operator=;
    };

}