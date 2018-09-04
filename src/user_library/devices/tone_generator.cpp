/* An implementation file for: */
#include "user_library/devices/tone_generator.hpp"

using namespace Cynth::UserLibrary::Devices;

ToneGenerator::ToneGenerator(
    WaveFunction f,
    float freq, // = 440
    float amp, // = 0
    freq_type_t freq_type) // = HZ
    : Oscillator(f, freq, amp, freq_type) {}

ToneGenerator::ToneGenerator(
    wave_func_t f,
    float freq, // = 440
    float amp, // = 0
    freq_type_t freq_type) // = HZ
    : Oscillator(f, freq, amp, freq_type) {}