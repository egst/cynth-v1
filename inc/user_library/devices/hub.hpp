#pragma once

/* Local libraries: */
#include "rack/devices/input_device.hpp"
#include "rack/devices/output_device.hpp"

namespace Cynth::UserLibrary::Devices {

    class Hub:
        public Rack::Devices::InputDevice,
        public Rack::Devices::OutputDevice {};
}