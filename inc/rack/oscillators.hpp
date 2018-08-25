#pragma once

/* Local libraries: */
#include "rack/devices.hpp"
// -> Cynth::InputDevice, Cynth::OutputDevice
#include "rack/functions.hpp"
// -> Cynth::WaveFunction, Cynth::SequenceFunction, Cynth::ConversionFunction

namespace Cynth {

    /*/ Frequency types: /*/
    enum freq_t {
        // Absolute frequency in Hertz:
        HZ,
        // Relative frequency in Measure Fractions:
        // 1 corresponds to one repetition per measure.
        // 1/2 to two repetitions per measure.
        // 2 to one repetition per two measures etc...
        MF,
        // Relative frequency in Measures per Minute:
        MPM,
        // Relative frequency in Beats per Minute:
        // One beat corresponds to a fourth note.
        BPM
    };

    /*/ Generic Oscillator device: /*/
    template<typename function_t>
    class OscillatorDevice: public InputDevice, public OutputDevice {
    protected:
        /* Properties: */
        // Frequency:
        float freq;
        // Amplitude:
        float amp;
        // Frequency type:
        freq_t freq_type;
        /* Wave Function: */
        function_t func;
        // Wave functions class for basic wavefunctions:
        WaveFs wave_fs;
        /* Frequency conversion function: */
        // Conversion functions class:
        ConvFs conv_fs;
        // Object:
        ConversionFunction<float, float> conv_f;
        // Method:
        float convertFrequency(float input);
        // For integer input:
        float convertFrequency(int input);
    
    public:
        /* Constructors: */
        // Without wave function:
        OscillatorDevice(
            float freq,
            float amp,
            freq_t freq_type);
        // With custom wave function:
        OscillatorDevice(
            function_t func,
            float freq,
            float amp,
            freq_t freq_type);
        // With basic wave function:
        OscillatorDevice(
            wave_t func,
            float freq,
            float amp,
            freq_t freq_type);

        /* Mutators: */
        void setFrequency(float freq);
        void setAmplitude(float freq);
        void setFunction(function_t func);

        /* Testing: */
        float play(float offset);
        void draw();
    };

    /*/ Sequencer Device:

        Used to generate rhythmic patters of (usually) discrete values.
        Output type is specific per instance. It may therefore be used
        to generate specific values for drum machines and similar devices.
    /*/
    template<typename output_t>
    class SequencerDevice: public OscillatorDevice<SequenceFunction<output_t>> {
    public:
        /* Constructors: */
        // With custom sequence function:
        SequencerDevice(
            SequenceFunction<output_t> seq_f,
            float freq = 1,
            float amp = 0,
            freq_t freq_type = MF);
        // TODO: With basic sequence function.
    };

    /*/ Tone Generator Device:

        Used to generade audible waves.
    /*/
    class ToneGeneratorDevice: public OscillatorDevice<WaveFunction> {
    public:
        /* Constructors: */
        // With custom wave function:
        ToneGeneratorDevice(
            WaveFunction wave_f,
            float freq = 440,
            float amp = 0,
            freq_t freq_type = HZ);
        // With basic wave function:
        ToneGeneratorDevice(
            wave_t wave_f,
            float freq = 440,
            float amp = 0,
            freq_t freq_type = HZ);
    };

    /*/ LFO Device:
        (Low Frequency Oscillator)

        Used to generade slower waves to modulate properties of other devices.
    /*/
    class LFODevice: public OscillatorDevice<WaveFunction> {
    public:
        /* Constructors: */
        // With custom wave function:
        LFODevice(
            WaveFunction wave_f,
            float freq = 1,
            float amp = 0,
            freq_t freq_type = MF);
        // TODO: With basic wave function.
    };
}