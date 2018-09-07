/* An implementation file for: */
#include "user_library/devices/sequencer.hpp"

using namespace Cynth::UserLibrary::Devices;
using Cynth::UserLibrary::Functions::SequenceFunction;

Sequencer::Sequencer() {
    this->freq_port << this->seq_function.freq_function;
    this->amp_port << this->seq_function.amp_function;
}

Sequencer& Sequencer::operator<<(SequenceFunction f) {
    this->seq_function = f;
}