#include "cynth.hpp"

#include <iostream>
#include <iomanip>
#include <exception>
#define _USE_MATH_DEFINES // -> M_PI
#include <cmath>
#include <string>

#include <Windows.h>


typedef struct tet12_t {
    int frequency;
    std::string note;
} tet12_t;

long mod(long a, long b) {
	return (a % b + b) % b;
}

const char* noteMap[] = {"a", "a#", "b", "c", "c#", "d", "d#", "e", "f", "f#", "g", "g#"};

const int tet12_ref_pos = 48;
const int tet12_ref_freq = 440;
const double k = 1.059463094;

tet12_t peakNote(int pos) {
    int rp = pos - tet12_ref_pos;

    int freq = tet12_ref_freq * std::pow(k, rp);

    int mod_note = mod(rp, 12);
    int oct_p = 9 + (rp);
    int oct_md = oct_p >= 0 ? 0 : -1;
    int mod_octave = 4 + std::floor( (oct_p - oct_md) / 12 ) + oct_md;

    std::string note(noteMap[mod_note]);
    note += std::to_string(mod_octave);

    return {
        .frequency = freq,
        .note = note
    };
}


#if defined(CYNTH_ENV_CLI)
int main(int argc, char* argv[]) {}
#elif defined(CYNTH_ENV_LIB_TESTING)

using namespace Cynth::UserLibrary::Devices;
using namespace Cynth::UserLibrary::Functions;
using Cynth::UserLibrary::Functions::wave_func_t;
using Cynth::UserLibrary::Functions::freq_type_t;

int main(int argc, char* argv[]) {
    try {
        WaveFs wave_fs_library;

        SoundCard sound_card;

        ToneGenerator tone_generator(wave_func_t::SAW, tet12_ref_freq, 0.33);
        sound_card.input_port << tone_generator.output_port;

        int pos = tet12_ref_pos;
        if (argc == 2) {
            pos = atoi(argv[1]);
        }

        tet12_t note = peakNote(pos);
        tone_generator.setFrequency(note.frequency);

        std::cout << "Press ESC to exit" << std::endl;

        std::cout << "Playing note: " << note.note << " " << '\r' << std::flush;

        sound_card.play();

        int oldpos = pos;

        bool aUP = false, aDOWN = false,
             pUP = false, pDOWN = false;

        while(true) {
            if ( (GetKeyState(VK_ESCAPE) & 0x8000) )
                break;

            if ( (GetKeyState(VK_UP) & 0x8000) )
                pUP = true;
            else if (!(GetKeyState(VK_UP) & 0x8000) && pUP) {
                pUP = false;
                aUP = false;
            }

            if ( (GetKeyState(VK_DOWN) & 0x8000) )
                pDOWN = true;
            else if (!(GetKeyState(VK_DOWN) & 0x8000) && pDOWN) {
                pDOWN = false;
                aDOWN = false;
            }
            

            if (pUP && !aUP) {
                pos++;
                aUP = true;
            }
            else if (pDOWN && !aDOWN) {
                pos--;
                aDOWN = true;
            }

            if (pos != oldpos) {
                note = peakNote(pos);
                tone_generator.setFrequency(note.frequency);
                std::cout << "Playing note: " << note.note << " " << '\r' << std::flush;
                oldpos = pos;
            }
        }

        sound_card.stop();
    }


    catch(Cynth::PlatformException& ex) { ex.log(); return EXIT_FAILURE; }
    #ifdef CYNTH_OS_WINDOWS
    catch(Cynth::COMException& ex) { ex.log(); return EXIT_FAILURE; }
    #endif
    #ifdef CYNTH_API_WASAPI
    catch(Cynth::WASAPIException& ex) { ex.log(); return EXIT_FAILURE; }
    #endif
    catch(Cynth::CynthException& ex) { ex.log(); return EXIT_FAILURE; }
    catch(std::exception& ex) { Cynth::Logger::log(ex.what()); return EXIT_FAILURE; }
}

#endif
