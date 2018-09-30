/* An implementation file for: */
#include "rack/components/transmitter_port.hpp"

using namespace Cynth::Rack::Components;

TransmitterPort::TransmitterPort() {}

TransmitterPort& TransmitterPort::operator<<(const WaveFunction& f) {
    this->ptr_wave_function = &f;
    return *this;
}

float TransmitterPort::operator()(double offset) const {
    return (*this->ptr_wave_function)(offset);
}