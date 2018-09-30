/* An implementation file for: */
#include "rack/components/receiver_port.hpp"

using namespace Cynth::Rack::Components;

ReceiverPort::ReceiverPort() {}

ReceiverPort& ReceiverPort::operator<<(const TransmitterPort& port) {
    this->ptr_transmitter_port = &port;
    return *this;
}

float ReceiverPort::operator()(double offset) const {
    return (*this->ptr_transmitter_port)(offset);
}