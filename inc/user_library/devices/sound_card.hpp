#pragma once

/* Local libraries: */
#include "platform.hpp"
#include "api/setup.hpp"
#include "api/device.hpp"
#include "rack/devices/output_device.hpp"

/* Standard libraries: */
#include <atomic>

namespace Cynth::UserLibrary::Devices {

    class SoundCard: public Cynth::Rack::Devices::OutputDevice {
    private:
        CYNTH_API_NS::Setup setup;
        CYNTH_API_NS::Device* ptr_rendering_device;
        // TODO: Deal with multiple rendering devices.
        int buffer_period_ms;
        int buffer_size_frames;
        int buffer_size_samples;
        int buffer_size_bytes;
        int channel_count;
        int bit_depth;
        int sample_rate;

        std::atomic<bool> stop_loop;
        void playLoop();
        void waitForBuffer();
        
    public:
        /* Constructor: */
        SoundCard();

        /* Debug: */
        void printProperties(); // TODO: Only in LIB and CLI

        void play();
    };

}