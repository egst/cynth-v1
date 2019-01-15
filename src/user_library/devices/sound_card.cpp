/* An implementation file for: */
#include "user_library/devices/sound_card.hpp"

/* Local libraries: */
#include "pcm/types.hpp"
#include "pcm/sample.hpp"
#include "pcm/buffer.hpp"
#include "logger.hpp"                                // Temporary
#include "user_library/functions/wave_functions.hpp" // Temporary

/* Standard libraries: */
#include <chrono>
#include <cmath>    // Temporary
#include <iostream> // Temporary

using namespace Cynth::UserLibrary::Devices;
using CYNTH_API_NS::Device;
using CYNTH_API_NS::Setup;
using time_units_t = CYNTH_API_NS::Device::time_units_t;
using size_units_t = CYNTH_API_NS::Device::size_units_t;
using byte_t = Cynth::PCM::byte_t;
using sample_t = Cynth::PCM::sample_t;
using Cynth::PCM::Sample;
using Cynth::PCM::Buffer;

SoundCard::SoundCard(): setup(), stop_loop(false) {
    this->ptr_rendering_device
        = &this->setup.getActiveRenderingDevices().front().get();
    // TODO: Deal with multiple rendering devices.
    this->buffer_period_ms
        = this->ptr_rendering_device->getBufferPeriodIn(time_units_t::MS);
    this->buffer_size_frames
        = this->ptr_rendering_device->getBufferSizeIn(size_units_t::FRAMES);
    this->buffer_size_samples
        = this->ptr_rendering_device->getBufferSizeIn(size_units_t::SAMPLES);
    this->buffer_size_bytes
        = this->ptr_rendering_device->getBufferSizeIn(size_units_t::BYTES);
    this->channel_count = this->ptr_rendering_device->getChannelCount();
    this->bit_depth = this->ptr_rendering_device->getBitDepth();
    this->sample_rate = this->ptr_rendering_device->getSampleRate();
}

void SoundCard::printProperties() {
    std::cout << "buffer period: " << buffer_period_ms << " ms" << std::endl;
    std::cout << "buffer size: " << buffer_size_frames << " frames" << std::endl;
    std::cout << "buffer size: " << buffer_size_samples << " samples" << std::endl;
    std::cout << "buffer size: " << buffer_size_bytes << " bytes" << std::endl;
    std::cout << "channel count: " << channel_count << std::endl;
    std::cout << "bit depth: " << bit_depth << " bits" << std::endl;
    std::cout << "sample rate: " << sample_rate << " Hz" << std::endl;
}

void SoundCard::waitForBuffer() {
    this->ptr_rendering_device->waitForBuffer();
}

void SoundCard::playLoop() {
    int frames_passed = 0;
    bool first = true;
    Sample sample(this->bit_depth);
    Buffer buffer(this->bit_depth);
    byte_t* ptr_buffer;
    int padded_buffer_size_frames;
    std::size_t padded_buffer_size_bytes;

    Cynth::UserLibrary::Functions::WaveFs wave_fs;
    
	while (!this->stop_loop) {
        std::thread wait;
        if (!first) {
            wait = std::thread(&SoundCard::waitForBuffer, this);
            //wait.join();
        }
        
        // Load full buffer:
        for (int i = 0; i < buffer_size_frames; i++) {
            /*float t
                = (double) this->channel_count
                * (i + periods_passed * padded_buffer_size_frames)
                / this->sample_rate;*/
            // TODO: Sample rate interpretation issues.
            float t = (double) (i + frames_passed) / this->sample_rate;
            sample = (float) this->input_port(t);
            for (int j = 0; j < this->channel_count; j++)
                buffer.write(sample);
        }

        if (!first)
            wait.join();

        ptr_buffer = this->ptr_rendering_device->getBuffer();
        // Get the actual buffer size with padding:
        padded_buffer_size_frames
            = this->ptr_rendering_device->getPaddedBufferSize();
        padded_buffer_size_bytes
            = this->ptr_rendering_device->getPaddedBufferSizeIn(
                size_units_t::BYTES);
        // Set offset for calculations in the next iteration:
        frames_passed += padded_buffer_size_frames;
        // Move to the padded buffer:
        buffer.moveTo(ptr_buffer, padded_buffer_size_bytes);

		this->ptr_rendering_device->releaseBuffer();

        if (first) {
            this->ptr_rendering_device->start();
            first = false;
        }
	}

    std::this_thread::sleep_for(
            std::chrono::milliseconds(this->buffer_period_ms / 2));

	this->ptr_rendering_device->stop();
}

void SoundCard::play() {
    // Start the loop:
    this->stop_loop = false;
    //std::thread loop(SoundCard::playLoop, this);
    this->loop_thread = std::thread(&SoundCard::playLoop, this);
    this->loop_thread.detach();

    /*// Temporary:
    std::cout << "Press any key to stop. ";
    std::cin.get();

    // Stop the loop:
    this->stop_loop = true;
    // Let the thread finish before going out of scope:
    loop.join();*/
}

void SoundCard::stop() {
    // Stop the loop:
    this->stop_loop = true;
    // Let the thread finish before going out of scope:
    //this->loop_thread.join();
}

