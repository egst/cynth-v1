/* Local libraries: */
#include "cynth.hpp"

/* Standard libraries: */
#include <iostream>
#include <exception>
#define _USE_MATH_DEFINES // -> M_PI
#include <cmath>

using namespace std;

#if defined(CYNTH_ENV_CLI)
int main(int argc, char* argv[]) {

    /* -- WASAPI testing: --------------------------------------------------- */

    try {
        CYNTH_API audio_API;
        Cynth::CLI CLI(audio_API);
    }
    catch(Cynth::PlatformException& ex) {
        ex.log();
        return EXIT_FAILURE;
    }
    #ifdef CYNTH_OS_WINDOWS
    catch(Cynth::COMException& ex) {
        ex.log();
        return EXIT_FAILURE;
    }
    #endif

    /* -- Cynth testing: ---------------------------------------------------- */

    cout << endl << endl << endl;
    
    /* Sound Card: */
    // Initiate:
    Cynth::SoundCard sound_card;
    // TODO: Setup API: Connect with the physical sound card etc.

    /* Tone Generator: */
    Cynth::ToneGeneratorDevice tone_generator1(Cynth::SINE);
    Cynth::ToneGeneratorDevice tone_generator2(Cynth::SQUARE);
    Cynth::ToneGeneratorDevice tone_generator3(Cynth::SAW);
    Cynth::ToneGeneratorDevice tone_generator4(Cynth::TRIANGLE);
    
    /*cout << "Sine Wave:" << endl;
    tone_generator1.draw();
    cout << endl;
    cout << "Square Wave:" << endl;
    tone_generator2.draw();
    cout << endl;
    cout << "Saw Wave:" << endl;
    tone_generator3.draw();
    cout << endl;
    cout << "Triangle Wave:" << endl;
    tone_generator4.draw();
    cout << endl;*/

    return 0;
}
#elif defined(CYNTH_ENV_LIB_TESTING)
using namespace Cynth::UserLibrary::Devices;
using Cynth::PCM::Functions::WaveFunction;
using Cynth::PCM::Functions::WaveFs;
using Cynth::PCM::Functions::wave_func_t;
using Cynth::PCM::Functions::freq_type_t;
int main() {
    try {
        SoundCard sound_card;
        ToneGenerator tone_generator(wave_func_t::SAW, 100, 0.1);
        //LFO lfo(wave_func_t::SINE, 1, 1); // TODO: Frequency types.
        WaveFs wave_fs_library;
        WaveFunction wobble;
        wobble = [wave_fs_library](float offset) -> float {
            return 0.3 * wave_fs_library.sine(offset) + 0.5;
        };
        LFO lfo(wobble, 0.5, 0.5, freq_type_t::HZ);
        tone_generator.amp_port << lfo.output_port;
        sound_card.input_port << tone_generator.output_port;
        sound_card.play();
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