#pragma once

/* Local libraries: */
#include "api/wasapi/interface.hpp"

/* Windows libraries: */
// Audio Client:
#include <audioclient.h>

namespace Cynth::API::WASAPI {

    class RenderClient: public Interface<IAudioRenderClient> {
    public:
        /* Constructor: */
        using Interface<IAudioRenderClient>::Interface;

        /* Friends: */
        friend class Device;
        friend class AudioClient;
    };

}