#pragma once

/* Local libraries: */
#include "user_library/functions/wave_function.hpp"
#include "user_library/functions/sequence_element.hpp"

/* Standard libraries: */
#include <vector>

namespace Cynth::UserLibrary::Functions {

    /*/ Sequence Function:
    
        Generates frequency values.
    /*/
    class SequenceFunction {
    private:
        double period;
        std::vector<SequenceElement> elements;
    public:
        SequenceFunction();

        //friend class Sequencer;
        WaveFunction freq_function;
        WaveFunction amp_function;

        /* Mutators: */
        // Add element:
        SequenceFunction& operator<<(SequenceElement element);
        // Add another function:
        SequenceFunction& operator<<(SequenceFunction f);
        // Repeat the current function:
        SequenceFunction& operator*=(int n);
    };

}