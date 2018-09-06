/* An implementation file for: */
#include "rack/devices/oscillator.hpp"

/* Standard library: */
#include <iostream> // temporary
#include <cmath>    // temporary

using namespace Cynth::Rack::Devices;
using Cynth::UserLibrary::Functions::WaveFunction;

float Oscillator::convertFrequency(float input) {
    return this->conv_f(input);
}
float Oscillator::convertFrequency(int input) {
    return this->convertFrequency((float) input);
}

Oscillator::Oscillator(
    float freq,
    float amp,
    freq_type_t freq_type)
    : freq_type(freq_type), wave_fs() {

    // Default constant frequency and amplitude:
    this->setFrequency(freq);
    this->setAmplitude(amp);
    
    // Modulated function:
    this->mod_function = [this](float offset) -> float {
        return
            this->amp_port(offset)
          * this->raw_function(offset * this->freq_port(offset));
    };

    // Output port:
    this->output_port << this->mod_function;

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

Oscillator::Oscillator(
    WaveFunction f,
    float freq,
    float amp,
    freq_type_t freq_type)
    : Oscillator(freq, amp, freq_type) {

    this->raw_function = f;
}

Oscillator::Oscillator(
    wave_func_t f,
    float freq,
    float amp,
    freq_type_t freq_type)
    : Oscillator(freq, amp, freq_type) {
    
    // Set the basic wave function:
    switch(f) {
    case wave_func_t::SINE:
        this->raw_function = this->wave_fs.sine;
        break;
    case wave_func_t::SINE_POS:
        this->raw_function = this->wave_fs.sine_pos;
        break;
    case wave_func_t::SQUARE:
        this->raw_function = this->wave_fs.square;
        break;
    case wave_func_t::SAW:
        this->raw_function = this->wave_fs.saw;
        break;
    case wave_func_t::TRIANGLE:
        this->raw_function = this->wave_fs.triangle;
        break;
    }
}

void Oscillator::setFrequency(float freq) {
    this->wave_fs.custom["freq"] = [freq](float offset) -> float {
        return freq;
    };
    this->freq_transmitter << this->wave_fs.custom["freq"];
    this->freq_port << this->freq_transmitter;
}

void Oscillator::setAmplitude(float amp) {
    this->wave_fs.custom["amp"] = [amp](float offset) -> float {
        return amp;
    };
    this->amp_transmitter << this->wave_fs.custom["amp"];
    this->amp_port << this->amp_transmitter;
}

void Oscillator::setFunction(WaveFunction f) {
    this->raw_function = f;
}