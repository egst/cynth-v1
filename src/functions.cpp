/* An implementation file for: */
#include "functions.hpp"

/* Local libraries: */
#include "tools.hpp"  // -> Cynth::Tools::compare()

/* Standard libraries: */
#define _USE_MATH_DEFINES // -> M_PI
#include <cmath>          // -> M_PI, std::sin()

/* -- Time Function: -------------------------------------------------------- */

template<typename output_t>
Cynth::TimeFunction<output_t>::TimeFunction() {}

template<typename output_t>
Cynth::TimeFunction<output_t>::TimeFunction(def_t def): def(def) {}

template<typename output_t>
output_t Cynth::TimeFunction<output_t>::operator() (float offset) {
    return this->def(offset);
}

template<typename output_t>
output_t Cynth::TimeFunction<output_t>::operator() (int offset) {
    return this->def((float) offset);
}

template<typename output_t>
void Cynth::TimeFunction<output_t>::setDef(def_t def) {
    this->def = def;
}

// Temporary solution:
// For WaveFunction.
template class Cynth::TimeFunction<float>;

/* -- Wave Function: -------------------------------------------------------- */

Cynth::WaveFunction::WaveFunction(): TimeFunction<float>() {}

Cynth::WaveFunction::WaveFunction(def_t def): TimeFunction<float>(def) {}

/* -- Basic wave functions: ------------------------------------------------- */

Cynth::WaveFs::WaveFs() {

    this->sine.setDef([](float offset) -> float {
        return std::sin(2 * M_PI * offset);
    });

    this->square.setDef([](float offset) -> float {
        if (Cynth::Tools::compare(offset, 0)
        ||  Cynth::Tools::compare(offset, 0.5)
        ||  Cynth::Tools::compare(offset, 1))
            return 0;
        else if (offset < 0.5)
            return 1;
        else
            return -1;
    });

    this->saw.setDef([](float offset) -> float {
        if (Cynth::Tools::compare(offset, 0.5))
            return 0;
        else if (offset < 0.5)
            return 2 * offset;
        else
            return 2 * offset - 2;
    });

    this->triangle.setDef([](float offset) -> float {
        if (offset < 0.25)
            return 4 * offset;
        if (Cynth::Tools::compare(offset, 0.25))
            return 1;
        else if (offset < 0.75)
            return -4 * offset + 2;
        if (Cynth::Tools::compare(offset, 0.75))
            return -1;
        else
            return 4 * offset - 4;
    });
}

/* -- Sequence Function: ---------------------------------------------------- */



/* -- Conversion Function: -------------------------------------------------- */

template<typename input_t, typename output_t>
Cynth::ConversionFunction<input_t, output_t>::ConversionFunction() {}

template<typename input_t, typename output_t>
Cynth::ConversionFunction<input_t, output_t>::ConversionFunction(def_t def): def(def) {}

template<typename input_t, typename output_t>
output_t Cynth::ConversionFunction<input_t, output_t>::operator() (input_t input) {
    return this->def(input);
}

template<typename input_t, typename output_t>
void Cynth::ConversionFunction<input_t, output_t>::setDef(def_t def) {
    this->def = def;
}

/* -- Frequency conversion functions: --------------------------------------- */

Cynth::ConvFs::ConvFs() {
    this->doNotConvert.setDef([](float input) -> float {
        return input;
    });

    this->convertMF.setDef([](float input) -> float {
        return input;
    });

    this->convertMPM.setDef([](float input) -> float {
        return input;
    });

    this->convertBPM.setDef([](float input) -> float {
        return input;
    });
}