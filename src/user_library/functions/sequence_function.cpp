/* An implementation file for: */
#include "user_library/functions/sequence_function.hpp"

/* Standard libraries: */
#include <cmath>

using namespace Cynth::UserLibrary::Functions;

SequenceFunction::SequenceFunction(): period(0) {
    this->freq_function = [this](float offset) -> float {
        std::fmod(offset, this->period);
        double t = 0;
        for (auto& element : this->elements) {
            if (t + element.duration > offset)
                return element.freq;
            t += element.duration;
        }
    };

    this->amp_function = [this](float offset) -> float {
        std::fmod(offset, this->period);
        double t = 0;
        for (auto& element : this->elements) {
            if (t + element.duration > offset)
                return element.amp;
            t += element.duration;
        }
    };
}

SequenceFunction& SequenceFunction::operator<<(SequenceElement element) {
    this->elements.push_back(element);
    this->period += element.duration;
}