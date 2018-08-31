/* An implementation file for: */
#include "pcm/functions/wave_function.hpp"

using namespace Cynth::PCM::Functions;

WaveFunction::WaveFunction(): TimeFunction<float>() {}

WaveFunction::WaveFunction(def_t def): TimeFunction<float>(def) {}