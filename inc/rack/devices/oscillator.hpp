#pragma once

/* Local libraries: */
#include "rack/devices/input_device.hpp"
#include "rack/devices/output_device.hpp"
#include "pcm/functions/wave_function.hpp"
#include "pcm/functions/wave_functions.hpp"
#include "pcm/functions/conversion_function.hpp"
#include "pcm/functions/conversion_functions.hpp"
#include "pcm/functions/sequence_function.hpp"

namespace Cynth::Rack::Devices {

    /*/ Generic Oscillator device: /*/
    template<typename function_t>
    class Oscillator: public InputDevice, public OutputDevice {
    protected:
        /* Aliases: */
        using freq_type_t = Cynth::PCM::Functions::freq_type_t;
        using wave_func_t = Cynth::PCM::Functions::wave_func_t;
        using WaveFunction = Cynth::PCM::Functions::WaveFunction;
        template<typename output_t>
        using SequenceFunction
            = Cynth::PCM::Functions::SequenceFunction<output_t>;
        template<typename input_t, typename output_t>
        using ConversionFunction
            = Cynth::PCM::Functions::ConversionFunction<input_t, output_t>;
        using WaveFs = Cynth::PCM::Functions::WaveFs;
        using ConvFs = Cynth::PCM::Functions::ConvFs;

        /* Properties: */
        // Frequency type:
        freq_type_t freq_type;
        // Frequency:
        float freq;
        // Amplitude:
        float amp;

        /* Wave Function: */
        function_t func;
        // Basic wave functions:
        WaveFs wave_fs;

        /* Frequency conversion function: */
        ConversionFunction<float, float> conv_f;
        // Basic conversion functions:
        ConvFs conv_fs;
        // Method:
        float convertFrequency(float input);
        float convertFrequency(int input);
    
    public:
        /* Constructors: */
        // Without wave function:
        Oscillator(
            float freq,
            float amp,
            freq_type_t freq_type);
        // With custom wave function:
        Oscillator(
            function_t func,
            float freq,
            float amp,
            freq_type_t freq_type);
        // With basic wave function:
        Oscillator(
            wave_func_t func,
            float freq,
            float amp,
            freq_type_t freq_type);

        /* Mutators: */
        void setFrequency(float freq);
        void setAmplitude(float freq);
        void setFunction(function_t func);

        /* Testing: */
        float play(float offset);
        void draw();
    };

}