/* Local libraries: */
#include "cynth.hpp"

/* Standard libraries: */
#include <iostream>
#include <exception>
#define _USE_MATH_DEFINES // -> M_PI
#include <cmath>

using namespace std;

#if defined(CYNTH_ENV_CLI)

int main(int argc, char* argv[]) {}

#elif defined(CYNTH_ENV_LIB_TESTING)

using namespace Cynth::UserLibrary::Devices;
using namespace Cynth::UserLibrary::Functions;
using Cynth::UserLibrary::Functions::wave_func_t;
using Cynth::UserLibrary::Functions::freq_type_t;

int main() {
    try {

        /* Workflow example: */

        // Library of basic wave functions:
        WaveFs wave_fs_library;

        // Sound card provides streaming to the output device:
        SoundCard sound_card;
        sound_card.printProperties();

        // Tone generator generates audible sound:
        ToneGenerator lead(wave_func_t::SINE);
        ToneGenerator bass(wave_func_t::SAW);

        // Custom wave function definition:
        WaveFunction wobble;
        wobble = [wave_fs_library](float offset) -> double {
            return (0.3 * wave_fs_library.sine(offset) + 0.5);
        };

        // LFO generates a wave function for modulating other devices:
        LFO lfo(wobble, 0.5, 0.5, freq_type_t::HZ);
        // TODO: Frequency types.

        // LFO controls tone generator's amplitude:
        //tone_generator.amp_port << lfo.output_port;

        // Sequencer controls tone generator's frequency and amplitude:
        Sequencer arpeggio;
        SequenceFunction seq_arp_1;
        seq_arp_1 << SequenceElement(0.25,  0.4, 261.63);   // C        1/4 ^
        seq_arp_1 << SequenceElement(0.125);                // Silence  1/8
        seq_arp_1 << SequenceElement(0.125, 0.2, 329.63);   // E        1/8
        seq_arp_1 << SequenceElement(0.375, 0.4, 392);      // G        3/8 ^
        seq_arp_1 << SequenceElement(0.125, 0.2, 493.88);   // B        1/8
        seq_arp_1 *= 2;                                     // Repeat 2x
        
        SequenceFunction seq_arp_2;
        seq_arp_2 << SequenceElement(0.25,  0.4, 233.08);   // A#       1/4 ^
        seq_arp_2 << SequenceElement(0.125);                // Silence  1/8
        seq_arp_2 << SequenceElement(0.125, 0.2, 329.63);   // E        1/8
        seq_arp_2 << SequenceElement(0.375, 0.4, 349.23);   // F        3/8 ^
        seq_arp_2 << SequenceElement(0.125, 0.2, 392);      // G        1/8
        seq_arp_2 *= 2;                                     // Repeat 2x

        // Merge:
        seq_arp_1 << seq_arp_2;

        arpeggio << seq_arp_1;
        lead.freq_port << arpeggio.freq_port;
        lead.amp_port << arpeggio.amp_port;

        Sequencer base;
        SequenceFunction seq_bas;
        seq_bas << SequenceElement(2, 0.1, 65.406);         // C        2
        seq_bas << SequenceElement(2, 0.1, 58.27);          // A#       2
        base << seq_bas;
        bass.freq_port << base.freq_port;
        bass.amp_port << base.amp_port;

        Hub hub_master(comp_type_t::ADD);
        hub_master << lead.output_port;
        hub_master << bass.output_port;

        // Tone generator outputs resulting modulated sound to sound card:
        sound_card.input_port << hub_master.output_port;

        // Sound card plays its input through the output device:
        //sound_card.play();
        //float freq_input;
        while (true) {
            /*std::cout << "Frequency: ";
            std::cin >> freq_input;
            tone_generator.setFrequency(freq_input);*/
            sound_card.play();
        }

    }
    /* Different exceptions separated for testing: */
    // Unsuported platform problems:
    catch(Cynth::PlatformException& ex) {
        ex.log();
        return EXIT_FAILURE;
    }
    #ifdef CYNTH_OS_WINDOWS
    // Component object model HRESULT error codes:
    catch(Cynth::COMException& ex) {
        ex.log();
        return EXIT_FAILURE;
    }
    #endif
    #ifdef CYNTH_API_WASAPI
    // WASAPI abstraction problems:
    catch(Cynth::WASAPIException& ex) {
        ex.log();
        return EXIT_FAILURE;
    }
    #endif
    // General problems:
    catch(Cynth::CynthException& ex) {
        ex.log();
        return EXIT_FAILURE;
    }
    // Default:
    catch(std::exception& ex) {
        Cynth::Logger::log(ex.what());
        return EXIT_FAILURE;
    }
}
#endif