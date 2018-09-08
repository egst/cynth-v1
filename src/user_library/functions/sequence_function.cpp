/* An implementation file for: */
#include "user_library/functions/sequence_function.hpp"

/* Standard libraries: */
#include <cmath>

using namespace Cynth::UserLibrary::Functions;

SequenceFunction::SequenceFunction(): period(0) {
    this->freq_function = [this](float offset) -> float {
        offset = std::fmod(offset, this->period);
        double t = 0;
        for (auto& element : this->elements) {
            if (t + element.duration > offset)
                return element.freq;
            t += element.duration;
        }
        return 0;
    };

    this->amp_function = [this](float offset) -> float {
        offset = std::fmod(offset, this->period);
        double t = 0;
        for (auto& element : this->elements) {
            if (t + element.duration > offset)
                return element.amp;
            t += element.duration;
        }
        return 0;
    };
}

SequenceFunction& SequenceFunction::operator<<(SequenceElement element) {
    this->elements.push_back(element);
    this->period += element.duration;
    return *this;
}

SequenceFunction& SequenceFunction::operator<<(SequenceFunction f) {
    for (auto& element : f.elements)
        *this << element;
    return *this;
}

SequenceFunction& SequenceFunction::operator*=(int n) {
    SequenceFunction copy;
    copy << *this;
    for (int i = 1; i < n; i++) {
        *this << copy;
    }
    return *this;
}