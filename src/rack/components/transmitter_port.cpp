/* An implementation file for: */
#include "rack/components/transmitter_port.hpp"

using namespace Cynth::Rack::Components;

TransmitterPort::TransmitterPort() {}

TransmitterPort& TransmitterPort::operator<<(WaveFunction& f) {
    this->ptr_wave_function = &f;
    return *this;
}

float TransmitterPort::operator()(float offset) {
    return (*this->ptr_wave_function)(offset);
}