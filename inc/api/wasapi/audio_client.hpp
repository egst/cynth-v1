#pragma once

/* Local libraries: */
#include "api/wasapi/interface.hpp"
#include "api/wasapi/render_client.hpp"

/* Windows libraries: */
// Audio Client:
#include <audioclient.h>

namespace Cynth::API::WASAPI {

    class AudioClient: public Interface<IAudioClient> {
    public:
        /* Constructor: */
        using Interface<IAudioClient>::Interface;

        /* Friends: */
        friend class Device;

        /* Related WASAPI interfaces: */
        RenderClient render_client;

        /* WASAPI methods abstractions: */
        void start();
        void stop();
    };

}