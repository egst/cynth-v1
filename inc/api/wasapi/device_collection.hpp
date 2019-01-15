#pragma once

/* Local libraries: */
#include "api/wasapi/interface.hpp"
#include "api/wasapi/device.hpp"

namespace Cynth::API::WASAPI {

    class DeviceCollection: public Interface<IMMDeviceCollection> {
      public:
        /* Constructor: */
        using Interface<IMMDeviceCollection>::Interface;

        /* WASAPI methods abstractions: */
        int getCount();
        Device item(UINT i);

        //tmp:
        IMMDeviceCollection* getPtr() {
            return this->ptr_instance;
        }
    };

}