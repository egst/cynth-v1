#pragma once

/* Standard libraries: */
#include <functional>

namespace Cynth::PCM::Functions {

    /*/ Frequency types: /*/
    enum freq_type_t {
        // Absolute frequency in Hertz:
        HZ,
        // Relative frequency in Measure Fractions:
        // 1 corresponds to one repetition per measure.
        // 1/2 to two repetitions per measure.
        // 2 to one repetition per two measures etc...
        MF,
        // Relative frequency in Measures per Minute:
        MPM,
        // Relative frequency in Beats per Minute:
        // One beat corresponds to a fourth note.
        BPM
    };

    /*/ Generic Time Function:
    
        For a given period offset returns the corresponding function value.
    /*/
    template<typename output_t = float>
    class TimeFunction {
    protected:
        /* Type definitions: */
        // Function definition:
        using def_t = std::function<output_t(float)>;

        /* Properties: */
        // Function definition:
        def_t def;
    
    public:
        /* Constructor: */
        TimeFunction();
        TimeFunction(def_t def);

        /* Accessors: */
        // Call the function:
        output_t operator() (float offset);
        // For int input:
        output_t operator() (int offset);

        /* Mutators: */
        // Set the function definition:
        TimeFunction<output_t>& operator=(def_t def);
    };

}