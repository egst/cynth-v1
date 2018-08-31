#pragma once

/* Local libraries: */
#include "api/wasapi/interface.hpp"
#include "api/wasapi/device_collection.hpp"
#include "api/wasapi/device.hpp"

namespace Cynth::API::WASAPI {

    class DeviceEnumerator: public Interface<IMMDeviceEnumerator> {
    public:
        /* Constructor:
        
        Creates WASAPI interface instance. */
        DeviceEnumerator();

        /* WASAPI methods abstractions: */
        DeviceCollection getDeviceCollection(
            EDataFlow data_flow = EDataFlow::eRender,
            DWORD mask = DEVICE_STATE_ACTIVE);
        Device getDefaultAudioEndpoint(
            EDataFlow data_flow = EDataFlow::eRender,
            ERole role = ERole::eMultimedia);
    };

}