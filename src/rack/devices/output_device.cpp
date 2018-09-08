/* An implementation file for: */
#include "rack/devices/output_device.hpp"

using namespace Cynth::Rack::Devices;

OutputDevice::OutputDevice() {}

OutputDevice& OutputDevice::operator<<(TransmitterPort& port) {
    this->input_port << port;
    return *this;
}