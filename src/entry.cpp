/* Local libraries: */
#include "cynth.hpp"

/* Standard libraries: */
#include <iostream>
#include <exception>

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
int main() {
    try {
        CYNTH_API audio_API;
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