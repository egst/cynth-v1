#pragma once

/* Local libraries: */
#include "platform.hpp"
#include "user_library/functions/wave_function.hpp"
#include "user_library/functions/sequence_element.hpp"

/* Standard libraries: */
#include <vector>

namespace Cynth::UserLibrary::Functions {

    /*/ Sequence Function:
    
        Generates frequency values.
    /*/
    class CYNTH_EXPORT SequenceFunction {
    private:
        double period;
        std::vector<SequenceElement> elements;
    public:
        SequenceFunction();

        //friend class Sequencer;
        // TODO: Should be priate?
        WaveFunction freq_function;
        WaveFunction amp_function;

        /* Mutators: */
        // Add element:
        SequenceFunction& operator<<(SequenceElement element);
        // Add another function:
        SequenceFunction& operator<<(SequenceFunction f);
        // Repeat the current function:
        SequenceFunction& operator*=(int n);
        // TODO: Binary * operation to repeat functions.
        // TODO: Binary + operation to concatenate functions?
    };

}