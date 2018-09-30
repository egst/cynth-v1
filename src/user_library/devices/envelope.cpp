/* An implementation file for: */
#include "user_library/devices/envelope.hpp"

using namespace Cynth::UserLibrary::Devices;

Envelope::Envelope() {
    this->env_function = [this](double offset) -> float {
        offset -= this->env_function.getStart();
        if (offset < 0) {
            return 0;
        } else if (offset < this->attack_time) {
            return (1 / this->attack_time) * offset;
        } else if (offset < this->decay_time) {
            return
                ((1 - this->sustain_level)
                / (this->attack_time - this->decay_time))
                * (offset - this->attack_time);
        } else {
            return this->sustain_level;
        }
        // TODO: Release.
        // TODO: Non-linear envelope.
    };

    this->mod_function = [this](double offset) -> float {
        return this->env_function(offset) * this->input_port(offset);
    };

    this->output_port << this->mod_function;
}