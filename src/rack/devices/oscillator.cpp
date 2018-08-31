/* An implementation file for: */
#include "rack/devices/oscillator.hpp"

/* Standard library: */
#include <iostream> // temporary
#include <cmath>    // temporary

using namespace Cynth::Rack::Devices;
using Cynth::PCM::Functions::WaveFunction;

template<typename function_t>
float Oscillator<function_t>::convertFrequency(float input) {
    return this->conv_f(input);
}
template<typename function_t>
float Oscillator<function_t>::convertFrequency(int input) {
    return this->convertFrequency((float) input);
}

template<typename function_t>
Oscillator<function_t>::Oscillator(
    float freq,
    float amp,
    freq_type_t freq_type)
    : freq_type(freq_type),
      freq(freq),
      amp(amp) {
    
    // Set the frequency conversion function:
    switch(freq_type) {
    case freq_type_t::HZ:
        this->conv_f = this->conv_fs.doNotConvert;
        break;
    case freq_type_t::MF:
        this->conv_f = this->conv_fs.convertMF;
        break;
    case freq_type_t::MPM:
        this->conv_f = this->conv_fs.convertMPM;
        break;
    case freq_type_t::BPM:
        this->conv_f = this->conv_fs.convertBPM;
        break;
    }
}

template<typename function_t>
Oscillator<function_t>::Oscillator(
    function_t func,
    float freq,
    float amp,
    freq_type_t freq_type)
    : Oscillator<function_t>(freq, amp, freq_type) {

    this->func = func;
}

template<>
Oscillator<WaveFunction>::Oscillator(
    wave_func_t func,
    float freq,
    float amp,
    freq_type_t freq_type)
    : Oscillator<WaveFunction>(freq, amp, freq_type) {
    
    // Set the basic wave function:
    switch(func) {
    case wave_func_t::SINE:
        this->func = this->wave_fs.sine;
        break;
    case wave_func_t::SQUARE:
        this->func = this->wave_fs.square;
        break;
    case wave_func_t::SAW:
        this->func = this->wave_fs.saw;
        break;
    case wave_func_t::TRIANGLE:
        this->func = this->wave_fs.triangle;
        break;
    }
}

template<typename function_t>
void Oscillator<function_t>::setFrequency(float freq) {
    this->freq = this->convertFrequency(freq);
}

template<typename function_t>
void Oscillator<function_t>::setAmplitude(float freq) {
    this->freq = freq;
}

template<typename function_t>
void Oscillator<function_t>::setFunction(function_t func) {
    this->func = func;
}

template<typename function_t>
float Oscillator<function_t>::play(float offset) {
    // Round to two decimal places.
    // +1 -1 to avoid negative zero.
    return (std::roundf(this->func(offset) * 100) / 100 + 1) - 1;
}

template<typename function_t>
void Oscillator<function_t>::draw() {
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

/* Types: */
template class Oscillator<Cynth::PCM::Functions::WaveFunction>;
template class Oscillator<Cynth::PCM::Functions::SequenceFunction<float>>;
template class Oscillator<Cynth::PCM::Functions::SequenceFunction<int>>;