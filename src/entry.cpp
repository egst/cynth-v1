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

        // Tone generator generates audible sound:
        ToneGenerator tone_generator(wave_func_t::SINE, 450, 0.1);

        // Custom wave function definition:
        WaveFunction wobble;
        wobble = [wave_fs_library](float offset) -> float {
            return 300 * (0.3 * wave_fs_library.sine(offset) + 0.5);
        };

        // LFO generates a wave function for modulating other devices:
        LFO lfo(wobble, 0.5, 0.5, freq_type_t::HZ);
        // TODO: Frequency types.

        // LFO controls tone generator's amplitude:
        //tone_generator.amp_port << lfo.output_port;

        // Tone generator outputs resulting modulated sound to sound card:
        sound_card.input_port << tone_generator.output_port;

        // Sound card plays its input through the output device:
        //sound_card.play();
        float freq_input;
        while (true) {
            std::cout << "Frequency: ";
            std::cin >> freq_input;
            tone_generator.setFrequency(freq_input);
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