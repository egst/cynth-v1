/* An implementation file for: */
#include "rack/components/receiver_port.hpp"

using namespace Cynth::Rack::Components;

ReceiverPort::ReceiverPort() {}

ReceiverPort& ReceiverPort::operator<<(TransmitterPort& port) {
    this->ptr_transmitter_port = &port;
    return *this;
}

float ReceiverPort::operator()(double offset) {
    return (*this->ptr_transmitter_port)(offset);
}