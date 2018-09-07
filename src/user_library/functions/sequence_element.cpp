/* An implementation file for: */
#include "user_library/functions/sequence_element.hpp"

using namespace Cynth::UserLibrary::Functions;

SequenceElement::SequenceElement(
    double duration,
    float amp, // = 0
    float freq) // = 0
    : duration(duration), freq(freq), amp(amp) {}