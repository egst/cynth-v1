/* An implementation file for: */
#include "user_library/devices/lfo.hpp"

using namespace Cynth::UserLibrary::Devices;

Cynth::UserLibrary::Devices::LFO::LFO(
    WaveFunction f,
    float freq, // = 1
    float amp, // = 0
    freq_type_t freq_type) // = MF
    : Oscillator(f, freq, amp, freq_type) {}

Cynth::UserLibrary::Devices::LFO::LFO(
    wave_func_t f,
    float freq, // = 1
    float amp, // = 0
    freq_type_t freq_type) // = MF
    : Oscillator(f, freq, amp, freq_type) {}