#pragma once

/* Local libraries: */
#include "pcm/functions/conversion_function.hpp"

namespace Cynth::PCM::Functions {

    /*/ Frequency conversion functions: /*/
    class ConvFs {
    public:
        /* Constructor: */
        // Initializes the functions.
        ConvFs();

        /* Functions: */
        // No conversion dummy function:
        ConversionFunction<float, float> doNotConvert;
        // Convert Measure Fractions to Hertz:
        ConversionFunction<float, float> convertMF;
        // Convert Measures per Minute to Hertz:
        ConversionFunction<float, float> convertMPM;
        // Convert Beats per Minute to Hertz:
        ConversionFunction<float, float> convertBPM;
    };

}