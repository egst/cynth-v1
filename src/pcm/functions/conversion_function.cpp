/* An implementation file for: */
#include "pcm/functions/conversion_function.hpp"

using namespace Cynth::PCM::Functions;

template<typename input_t, typename output_t>
ConversionFunction<input_t, output_t>::ConversionFunction() {}

template<typename input_t, typename output_t>
ConversionFunction<input_t, output_t>::ConversionFunction(def_t def)
    : def(def) {}

template<typename input_t, typename output_t>
output_t ConversionFunction<input_t, output_t>::operator()(input_t input) {
    return this->def(input);
}

template<typename input_t, typename output_t>
ConversionFunction<input_t, output_t>&
ConversionFunction<input_t, output_t>::operator=(def_t def) {
    this->def = def;
    return *this;
}

/* Types: */
template class Cynth::PCM::Functions::ConversionFunction<float, float>;
template class Cynth::PCM::Functions::ConversionFunction<float, int>;
template class Cynth::PCM::Functions::ConversionFunction<int, float>;