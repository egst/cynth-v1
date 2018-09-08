/* An implementation file for: */
#include "user_library/devices/hub.hpp"

using namespace Cynth::UserLibrary::Devices;

Hub::Hub(comp_type_t comp) { // = comp_type_t::ADD
    switch (comp) {
    case comp_type_t::ADD:
        this->comp_function = [this](double offset) -> float {
            float amp = 0;
            for (auto& port : this->input_ports) {
                amp += port(offset);
            }
            return amp;
        };
        break;
    case comp_type_t::MULTIP:
        this->comp_function = [this](double offset) -> float {
            float amp = 1;
            for (auto& port : this->input_ports) {
                amp *= port(offset);
            }
            return amp;
        };
        break;
    }
    this->output_port << this->comp_function;
}

Hub& Hub::operator<<(TransmitterPort& port) {
    this->input_ports.emplace_back();
    this->input_ports.back() << port;
    return *this;
}