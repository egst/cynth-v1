/* An implementation file for: */
#include "user_library/devices/sequencer.hpp"

Cynth::UserLibrary::Devices::Sequencer::Sequencer(
    // Params:
    SequenceFunction<float> seq_f,
    float freq, // = 1
    float amp, // = 0
    freq_type_t freq_type) // = MF
    : Oscillator<SequenceFunction<float>>(seq_f, freq, amp, freq_type) {}