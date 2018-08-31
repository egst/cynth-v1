/* An implementation file for: */
#include "pcm/functions/time_function.hpp"

using namespace Cynth::PCM::Functions;

template<typename output_t>
TimeFunction<output_t>::TimeFunction() {}

template<typename output_t>
TimeFunction<output_t>::TimeFunction(def_t def): def(def) {}

template<typename output_t>
output_t TimeFunction<output_t>::operator() (float offset) {
    return this->def(offset);
}

template<typename output_t>
output_t TimeFunction<output_t>::operator() (int offset) {
    return this->def((float) offset);
}

template<typename output_t>
TimeFunction<output_t>& TimeFunction<output_t>::operator=(def_t def) {
    this->def = def;
    return *this;
}

/* Types: */
template class TimeFunction<float>;
template class TimeFunction<int>;