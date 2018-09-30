class __declspec(dllimport) Foo {
public:
    static void test();
};

#include <iostream>
#define _USE_MATH_DEFINES // -> M_PI
#include <cmath>
#include <thread>

using namespace std;

/*
using namespace Cynth::UserLibrary;
using namespace Cynth::UserLibrary::Devices;
using namespace Cynth::UserLibrary::Functions;
using wave_func_t = Types::wave_func_t;
using freq_type_t = Types::freq_type_t;
using comp_type_t = Types::comp_type_t;
*/

void test() {
    Foo::test();
}

int main() {
    cout << "e";
    test();
    //thread thread_test(test);
    //thread_test.detach();
    #if 0
    try {
        /* Workflow example: */
        
        // Cynth uses internal wave function abstraction - class WaveFunction.
        // Cynth provides a library of basic wave functions:
        //WaveFs wave_fs_library;

        // A custom WaveFunction may be defined with a lambda function:
        /*WaveFunction sine_pos;
        sine_pos = [wave_fs_library](double offset) -> float {
            return wave_fs_library.sine_pos(offset - (M_PI / 4));
        };

        // Cynth provides a set of basic devices:
        // SoundCard
        // ToneGenerator
        // LFO
        // Sequencer

        // Sound card provides streaming to the output device:
        SoundCard sound_card;
        sound_card.printProperties();

        // Tone generators generate audible sounds:
        ToneGenerator lead(wave_func_t::SAW);
        ToneGenerator bass(wave_func_t::SAW);

        // LFOs generate low frequency waves to modulate other devices:
        LFO lfo_vibrato(sine_pos, 10, 0.5, freq_type_t::HZ);
        LFO lfo_wah(sine_pos, 1, 0.5, freq_type_t::HZ);
        // TODO: Frequency types.

        // Cynth provides SequenceFunction - a wrapper for two wave functions:
        // One for frequency and one for amplitude.

        // A custom sequence function may be defined using SequenceElement -
        // an abstraction of a tone with its duration, frequency and amplitude:
        // Sequence 1:
        SequenceFunction seq_arp_1;
        seq_arp_1 << SequenceElement(0.25,  0.4, 261.63);   // C        1/4 ^
        seq_arp_1 << SequenceElement(0.125);                // Silence  1/8
        seq_arp_1 << SequenceElement(0.125, 0.2, 329.63);   // E        1/8
        seq_arp_1 << SequenceElement(0.375, 0.4, 392);      // G        3/8 ^
        seq_arp_1 << SequenceElement(0.125, 0.2, 493.88);   // B        1/8
        seq_arp_1 *= 2;                                     // Repeat 2x
        // Sequence 2:
        SequenceFunction seq_arp_2;
        seq_arp_2 << SequenceElement(0.25,  0.4, 233.08);   // A#       1/4 ^
        seq_arp_2 << SequenceElement(0.125);                // Silence  1/8
        seq_arp_2 << SequenceElement(0.125, 0.2, 329.63);   // E        1/8
        seq_arp_2 << SequenceElement(0.375, 0.4, 349.23);   // F        3/8 ^
        seq_arp_2 << SequenceElement(0.125, 0.2, 392);      // G        1/8
        seq_arp_2 *= 2;                                     // Repeat 2x
        // Append sequence 2 to sequence 1:
        seq_arp_1 << seq_arp_2;

        // Sequencer generates frequencies and amplitudes
        // based on the given sequence function:
        Sequencer arpeggio;
        arpeggio << seq_arp_1;

        // Hub with MULTIP setting merges effects:
        Hub hub_vibrato(comp_type_t::MULTIP);
        hub_vibrato << arpeggio.amp_port << lfo_vibrato.output_port;
        // This one will provide amplitude modulation.
        // LFO's output is accesible via its output_port.
        // Sequencer's amplitude output is accessible via its amp_port.

        // Oscillators' (Tone generators and LDOs) frequency and amplitude
        // may be modulated via their freq_port and amp_port respectively:
        lead.freq_port << arpeggio.freq_port;
        lead.amp_port << hub_vibrato.output_port;
        // Hub's output is accessible via its output_port.
        // Sequencer's frequency output is accessible via its freq_port.

        // Another sequence function:
        SequenceFunction seq_bas;
        seq_bas << SequenceElement(2, 0.2, 65.406);         // C        2
        seq_bas << SequenceElement(2, 0.2, 58.27);          // A#       2
        // Another sequencer:
        Sequencer base;
        base << seq_bas;
        // Another Hub:
        Hub hub_wah(comp_type_t::MULTIP);
        hub_wah << base.amp_port << lfo_wah.output_port;
        // Another tone generator modulated:
        bass.freq_port << base.freq_port;
        bass.amp_port << hub_wah.output_port;

        // Hub with ADD setting (default) merges audible sounds:
        Hub hub_master(comp_type_t::ADD);
        hub_master << lead.output_port << bass.output_port;

        // Sound card streams the given audible sound:
        sound_card << hub_master.output_port;

        // Sound card's play() method starts the playback.
        sound_card.play();*/

    }
    /* Different exceptions separated for testing: */
    // Unsuported platform problems:
    catch(Cynth::PlatformException& ex) { ex.log(); return EXIT_FAILURE; }
    #ifdef CYNTH_OS_WINDOWS
    // Component object model HRESULT error codes:
    catch(Cynth::COMException& ex) { ex.log(); return EXIT_FAILURE; }
    #endif
    #ifdef CYNTH_API_WASAPI
    // WASAPI abstraction problems:
    catch(Cynth::WASAPIException& ex) { ex.log(); return EXIT_FAILURE; }
    #endif
    // General problems:
    catch(Cynth::CynthException& ex) { ex.log(); return EXIT_FAILURE; }
    // Default:
    catch(std::exception& ex) {
        cout << ex.what();
        return EXIT_FAILURE;
    }
    #endif
}