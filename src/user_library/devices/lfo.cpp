/* An implementation file for: */
#include "user_library/devices/lfo.hpp"

Cynth::UserLibrary::Devices::LFO::LFO(
    WaveFunction wave_f,
    float freq, // = 1
    float amp, // = 0
    freq_type_t freq_type) // = MF
    : Oscillator<WaveFunction>(wave_f, freq, amp, freq_type) {}