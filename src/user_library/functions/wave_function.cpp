/* An implementation file for: */
#include "user_library/functions/wave_function.hpp"

using namespace Cynth::UserLibrary::Functions;

WaveFunction::WaveFunction(): TimeFunction<float>() {}

WaveFunction::WaveFunction(def_t def): TimeFunction<float>(def) {}