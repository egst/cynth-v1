#pragma once

/* Local libraries: */
#include "api/wasapi/interface.hpp"
#include "api/wasapi/audio_client.hpp"
#include "api/wasapi/property_store.hpp"

/* Windows libraries: */
// Multimedia Device API:
#include <mmdeviceapi.h>

/* Standard libraries: */
#include <string>

namespace Cynth::API::WASAPI {

    class Device: public Interface<IMMDevice> {
    private:
        /* Properties: */
        // Display name:
        std::string name;

        /* WASAPI properties: */
        // TODO: Type aliases needed?
        REFERENCE_TIME default_device_period;
        REFERENCE_TIME minimum_device_period;
        // WAVFORMATEX may contain extra bytes after
        // to form WAVEFORMATEXTENSIBLE. Number of these extra bytes
        // is stored in wave_format->cbSize. The extra information
        // may be needed for further WASAPI methods.
        // So as not to copy the information manually from memory,
        // only a pointer is stored to the received WAVEFORMATEX,
        // which is followed by these potential extra bytes.
        // TODO: For cleanup, is it needed to delete these extra bytes?
        WAVEFORMATEX* wave_format; // TODO: Rename to ptr_wave_format
        UINT32 buffer_size_frames;
        INT32 buffer_size_bytes;

        /* WASAPI methods abstractions: */
        void getDevicePeriod();
        void getBufferSize();
        void getMixFormat();
        void checkFormatSupport();
        AudioClient getAudioClient();
        void initAudioClient();
        RenderClient getRenderClient();

    public:
        /* Constructor: */
        Device(IMMDevice* ptr_instance = nullptr);

        /* Audio Client: */
        // Activate and setup Audio Client:
        void setup();
        // Release AudioClient:
        void releaseAudioClient();

        /* Friends: */
        friend class Control;

        /* Mutators: */
        void setName(std::string name);

        /* Related WASAPI interfaces: */
        // Audio Client:
        AudioClient audio_client;

        /* WASAPI methods abstractions: */
        PropertyStore openPropertyStore(DWORD access = STGM_READ);
        std::string getId();
    };

}