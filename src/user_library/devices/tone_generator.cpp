/* An implementation file for: */
#include "user_library/devices/tone_generator.hpp"

Cynth::UserLibrary::Devices::ToneGenerator::ToneGenerator(
    WaveFunction wave_f,
    float freq, // = 440
    float amp, // = 0
    freq_type_t freq_type) // = HZ
    : Oscillator<WaveFunction>(wave_f, freq, amp, freq_type) {}

Cynth::UserLibrary::Devices::ToneGenerator::ToneGenerator(
    wave_func_t wave_f,
    float freq, // = 440
    float amp, // = 0
    freq_type_t freq_type) // = HZ
    : Oscillator<WaveFunction>(wave_f, freq, amp, freq_type) {}