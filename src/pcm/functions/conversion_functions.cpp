/* An implementation file for: */
#include "pcm/functions/conversion_functions.hpp"

using namespace Cynth::PCM::Functions;

ConvFs::ConvFs() {
    this->doNotConvert = [](float input) -> float {
        return input;
    };

    this->convertMF = [](float input) -> float {
        return input;
    };

    this->convertMPM = [](float input) -> float {
        return input;
    };

    this->convertBPM = [](float input) -> float {
        return input;
    };
}