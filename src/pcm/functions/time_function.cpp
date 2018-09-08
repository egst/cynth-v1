/* An implementation file for: */
#include "pcm/functions/time_function.hpp"

using namespace Cynth::PCM::Functions;

template<typename output_t>
TimeFunction<output_t>::TimeFunction() {}

template<typename output_t>
TimeFunction<output_t>::TimeFunction(def_t def): def(def) {}

template<typename output_t>
output_t TimeFunction<output_t>::operator()(double offset) const {
    return this->def(offset);
}

template<typename output_t>
double TimeFunction<output_t>::getStart() const {
    return this->start;
}

template<typename output_t>
void TimeFunction<output_t>::setStart(double offset) {
    this->start = offset;
}

template<typename output_t>
TimeFunction<output_t>& TimeFunction<output_t>::operator=(def_t def) {
    this->def = def;
    return *this;
}

/* Types: */
template class TimeFunction<float>;
template class TimeFunction<int>;