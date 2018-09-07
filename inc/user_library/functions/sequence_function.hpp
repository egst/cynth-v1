#pragma once

/* Local libraries: */
#include "user_library/functions/wave_function.hpp"
#include "user_library/functions/sequence_element.hpp"
#include "user_library/devices/sequencer.hpp"

/* Standard libraries: */
#include <vector>

namespace Cynth::UserLibrary::Functions {

    /*/ Sequence Function:
    
        Generates frequency values.
    /*/
    class SequenceFunction {
    private:
        using Sequencer = Cynth::UserLibrary::Devices::Sequencer;
        double period;
        std::vector<SequenceElement> elements;
    public:
        SequenceFunction();

        //friend class Sequencer;
        WaveFunction freq_function;
        WaveFunction amp_function;

        SequenceFunction& operator<<(SequenceElement element);
    };

}