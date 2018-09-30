#pragma once

/* Local libraries: */
#include "rack/components/transmitter_port.hpp"
#include "rack/components/receiver_port.hpp"
#include "rack/devices/input_device.hpp"
#include "rack/devices/output_device.hpp"
#include "pcm/functions/conversion_function.hpp"
#include "pcm/functions/conversion_functions.hpp"
#include "user_library/types.hpp"
#include "user_library/functions/wave_function.hpp"
#include "user_library/functions/wave_functions.hpp"
#include "user_library/functions/sequence_function.hpp"

namespace Cynth::Rack::Devices {

    /*/ Generic Oscillator device: /*/
    class Oscillator: public InputDevice {
    protected:
        /* Aliases: */
        using ReceiverPort = Cynth::Rack::Components::ReceiverPort;
        using TransmitterPort = Cynth::Rack::Components::TransmitterPort;
        using freq_type_t = Cynth::UserLibrary::Types::freq_type_t;
        using wave_func_t = Cynth::UserLibrary::Types::wave_func_t;
        using WaveFunction = Cynth::UserLibrary::Functions::WaveFunction;
        using SequenceFunction
            = Cynth::UserLibrary::Functions::SequenceFunction;
        template<typename input_t, typename output_t>
        using ConversionFunction
            = Cynth::PCM::Functions::ConversionFunction<input_t, output_t>;
        using WaveFs = Cynth::UserLibrary::Functions::WaveFs;
        using ConvFs = Cynth::PCM::Functions::ConvFs;

        // Frequency type:
        freq_type_t freq_type;

        /* Wave Function: */
        // Raw function:
        WaveFunction raw_function;
        // Modulated function = amp(t) * f(t * freq(t)):
        WaveFunction mod_function;
        // Basic wave functions library:
        WaveFs wave_fs;

        /* Default transmitter ports for constant frequency and amplitude: */
        TransmitterPort freq_transmitter;
        TransmitterPort amp_transmitter;

        /* Frequency conversion function: */
        // TODO...
        ConversionFunction<float, float> conv_f;
        // Basic conversion functions:
        ConvFs conv_fs;
        // Method:
        float convertFrequency(float input);
        float convertFrequency(int input);
    
    public:
        /* Modulation ports: */
        // Frequency:
        ReceiverPort freq_port;
        // Amplitude:
        ReceiverPort amp_port;

        /* Constructors: */
        // Without wave function:
        Oscillator(
            float freq,
            float amp,
            freq_type_t freq_type);
        // With custom wave function:
        Oscillator(
            WaveFunction func,
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
        void setFunction(WaveFunction func);
        void setFunction(wave_func_t func);
    };

}