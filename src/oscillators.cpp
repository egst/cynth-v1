/* An implementation file for: */
#include "rack/oscillators.hpp"

/* Standard Libraries: */
#include <cmath>    // -> std::roundf (temporary)
#include <iostream> // -> std::cout, std::cin, std::endl (temporary)

/* -- Generic Oscillator device: -------------------------------------------- */

template<typename function_t>
float Cynth::OscillatorDevice<function_t>::convertFrequency(float input) {
    return this->conv_f(input);
}
template<typename function_t>
float Cynth::OscillatorDevice<function_t>::convertFrequency(int input) {
    return this->convertFrequency((float) input);
}

template<typename function_t>
Cynth::OscillatorDevice<function_t>::OscillatorDevice(
    // Params:
    float freq,
    float amp,
    Cynth::freq_t freq_type):
    // Init:
    freq(freq),
    amp(amp),
    freq_type(freq_type) {
    
    // Set the frequency conversion function:
    switch(freq_type) {
    case HZ:
        this->conv_f = this->conv_fs.doNotConvert;
        break;
    case MF:
        this->conv_f = this->conv_fs.convertMF;
        break;
    case MPM:
        this->conv_f = this->conv_fs.convertMPM;
        break;
    case BPM:
        this->conv_f = this->conv_fs.convertBPM;
        break;
    }
}

template<typename function_t>
Cynth::OscillatorDevice<function_t>::OscillatorDevice(
    // Params:
    function_t func,
    float freq,
    float amp,
    Cynth::freq_t freq_type):
    // Init:
    OscillatorDevice<function_t>(freq, amp, freq_type) {
    this->func = func;
}

template<typename function_t>
Cynth::OscillatorDevice<function_t>::OscillatorDevice(
    // Params:
    Cynth::wave_t func,
    float freq,
    float amp,
    Cynth::freq_t freq_type):
    // Init:
    OscillatorDevice<function_t>(freq, amp, freq_type) {
    
    // Set the basic wave function:
    switch(func) {
    case SINE:
        this->func = this->wave_fs.sine;
        break;
    case SQUARE:
        this->func = this->wave_fs.square;
        break;
    case SAW:
        this->func = this->wave_fs.saw;
        break;
    case TRIANGLE:
        this->func = this->wave_fs.triangle;
        break;
    }
}

template<typename function_t>
void Cynth::OscillatorDevice<function_t>::setFrequency(float freq) {
    this->freq = this->convertFrequency(freq);
}

template<typename function_t>
void Cynth::OscillatorDevice<function_t>::setAmplitude(float freq) {
    this->freq = freq;
}

template<typename function_t>
void Cynth::OscillatorDevice<function_t>::setFunction(function_t func) {
    this->func = func;
}

template<typename function_t>
float Cynth::OscillatorDevice<function_t>::play(float offset) {
    // Round to two decimal places.
    // +1 -1 to avoid negative zero.
    return (std::roundf(this->func(offset) * 100) / 100 + 1) - 1;
}

template<typename function_t>
void Cynth::OscillatorDevice<function_t>::draw() {
    for (int i = 0; i <= 20; i++) {
        int pos = roundf(this->func((float) i / 20) * 20);
        for (int j = -20; j <= 20; j++) {
            if (j == pos)
                std::cout << "O";
            else
                std::cout << " ";
        }
        std::cout << std::endl;
    }
}

// Temporary solution:
// For ToneGenerator.
template void Cynth::OscillatorDevice<Cynth::WaveFunction>::draw();

/* -- Sequencer Device: ----------------------------------------------------- */

template<typename output_t>
Cynth::SequencerDevice<output_t>::SequencerDevice(
    // Params:
    Cynth::SequenceFunction<output_t> seq_f,
    float freq, // = 1
    float amp, // = 0
    Cynth::freq_t freq_type): // = MF
    // Init:
    Cynth::OscillatorDevice<Cynth::SequenceFunction<output_t>>(
        seq_f,
        freq,
        amp,
        freq_type
    ) {}

/* -- Tone Generator Device: ------------------------------------------------ */

Cynth::ToneGeneratorDevice::ToneGeneratorDevice(
    // Params:
    Cynth::WaveFunction wave_f,
    float freq, // = 440
    float amp, // = 0
    Cynth::freq_t freq_type): // = HZ
    // Init:
    OscillatorDevice<WaveFunction>(wave_f, freq, amp, freq_type) {}

Cynth::ToneGeneratorDevice::ToneGeneratorDevice(
    // Params:
    wave_t wave_f,
    float freq, // = 440
    float amp, // = 0
    Cynth::freq_t freq_type): // = HZ
    // Init:
    OscillatorDevice<WaveFunction>(wave_f, freq, amp, freq_type) {}

/* -- LFO Device: ----------------------------------------------------------- */

Cynth::LFODevice::LFODevice(
    // Params:
    Cynth::WaveFunction wave_f,
    float freq, // = 1
    float amp, // = 0
    Cynth::freq_t freq_type): // = MF
    // Init:
    OscillatorDevice<WaveFunction>(wave_f, freq, amp, freq_type) {}