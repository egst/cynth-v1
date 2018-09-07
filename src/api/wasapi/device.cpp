/* An implementation file for: */
#include "api/wasapi/device.hpp"

/* Local libraries: */
#include "logger.hpp"
#include "tools.hpp"

/* Standard libraries: */
#include <iostream> // Temporary

using namespace Cynth::API::WASAPI;
using Cynth::Logger;
using byte_t = Cynth::PCM::byte_t;

Device::Device(IMMDevice* ptr_instance) // = nullptr
    : Interface<IMMDevice>(ptr_instance) {}

void Device::setup() {
    if (! this->ptr_instance)
        Logger::errWASAPI(
            "Pointer to COM interface not initialized "
            "before activating audio client.");
    this->audio_client = this->getAudioClient();
    this->audio_client.autoRelease();
    this->getMixFormat();
    // TODO: Get amplitude format.
    /*WAVEFORMATEXTENSIBLE* ptr_wave_format_extensible
        = (WAVEFORMATEXTENSIBLE*) this->ptr_wave_format;
    std::string sub_format;
    sub_format
        = Cynth::Tools::guidToString(ptr_wave_format_extensible->SubFormat);
    Logger::log(sub_format);
    Logger::errCynth("STOP");*/
    this->checkFormatSupport();
    this->getDevicePeriod();
    this->initAudioClient();
    this->audio_client.setEventHandle();
    this->getBufferSize();
    this->audio_client.render_client = this->getRenderClient();
}

void Device::getMixFormat() {
    WAVEFORMATEX* ptr_mix_format;
    HRESULT hr = this->audio_client->GetMixFormat(
        &ptr_mix_format);
    if (FAILED(hr))
        Logger::errHRESULT(hr);
    this->ptr_wave_format = ptr_mix_format;
    this->channel_count = this->ptr_wave_format->nChannels;
    this->bit_depth = this->ptr_wave_format->wBitsPerSample;
    this->sample_rate = this->ptr_wave_format->nSamplesPerSec;
}

AudioClient Device::getAudioClient() {
    // TODO: Problems with intellisense:
    #ifndef __INTELLISENSE__
    IID IID_IAudioClient = __uuidof(IAudioClient);
    #endif
    IAudioClient* ptr_IAudioClient;
    HRESULT hr = this->ptr_instance->Activate(
        IID_IAudioClient,
        CLSCTX_ALL,
        NULL,
        (void**) &ptr_IAudioClient);
    if (FAILED(hr))
        Logger::errHRESULT(hr);
    AudioClient audio_client(ptr_IAudioClient);
    audio_client.noAutoRelease();
    return audio_client;
}

void Device::initAudioClient() {
    HRESULT hr = this->audio_client->Initialize(
        AUDCLNT_SHAREMODE_SHARED,
        AUDCLNT_STREAMFLAGS_EVENTCALLBACK,
        this->default_device_period,
        0,
        this->ptr_wave_format,
        NULL); // TODO: How to generate session GUID?
    if (FAILED(hr))
        Logger::errHRESULT(hr);
}

RenderClient Device::getRenderClient() {
    // TODO: Intellisense problems:
    #ifndef __INTELLISENSE__
    const IID IID_IAudioRenderClient = __uuidof(IAudioRenderClient);
    #endif
    IAudioRenderClient* ptr_IAudioRenderClient;
    HRESULT hr = this->audio_client->GetService(
        IID_IAudioRenderClient,
        (void**) &ptr_IAudioRenderClient);
    if (FAILED(hr))
        Logger::errHRESULT(hr);
    RenderClient render_client(ptr_IAudioRenderClient);
    render_client.noAutoRelease();
    return render_client;
}

void Device::releaseAudioClient() {
    // TODO: Better abstraction.
    this->audio_client.render_client->Release();
    this->audio_client->Release();
    this->audio_client = nullptr;
}

void Device::getDevicePeriod() {
    HRESULT hr = this->audio_client->GetDevicePeriod(
        &this->default_device_period,
        &this->minimum_device_period);
    if (FAILED(hr))
        Logger::errHRESULT(hr);
    // Temporary:
    //this->default_device_period *= 2;
}

void Device::getBufferSize() {
    UINT32 buffer_size;
    HRESULT hr = this->audio_client->GetBufferSize(&buffer_size);
    if (FAILED(hr))
        Logger::errHRESULT(hr);
    this->buffer_size_frames = buffer_size;
    this->buffer_size_bytes =
        this->buffer_size_frames
        * this->ptr_wave_format->nChannels
        * (this->ptr_wave_format->wBitsPerSample / 8);
}

void Device::checkFormatSupport() {
    if (! this->audio_client.ptr_instance)
        Logger::errWASAPI(
            "Pointer to COM interface not initialized "
            "before checking format support.");
    WAVEFORMATEX* ptr_wave_format = this->ptr_wave_format;
    WAVEFORMATEX* ptr_closest_format = nullptr;
    HRESULT hr = this->audio_client->IsFormatSupported(
        AUDCLNT_SHAREMODE_SHARED,
        ptr_wave_format,
        &ptr_closest_format);
    // TODO: if (hr == AUDCLNT_E_UNSUPPORTED_FORMAT) {}
    if (FAILED(hr)) {
        Logger::errHRESULT(hr);
    }
    if (ptr_closest_format) {
        this->ptr_wave_format = ptr_closest_format;
    }
}

void Device::setName(std::string name) {
    this->name = name;
}

PropertyStore Device::openPropertyStore(DWORD access) { // = STGM_READ
    IPropertyStore* ptr_IPropertyStore;
    HRESULT hr = this->ptr_instance->OpenPropertyStore(access, &ptr_IPropertyStore);
    if (FAILED(hr))
        Logger::errHRESULT(hr);
    PropertyStore property_store(ptr_IPropertyStore);
    property_store.noAutoRelease();
    return property_store;
}

std::string Device::getId() {
    LPWSTR id_wchar;
    this->ptr_instance->GetId(&id_wchar);
    std::string id_str;
    id_str = Cynth::Tools::wcharToString(id_wchar);
    return id_str;
}

int Device::getBufferPeriodIn(time_units_t units) {
    switch(units) {
    case time_units_t::MS:
        return this->default_device_period / 10000;
    case time_units_t::HNS:
        return this->default_device_period;
    default:
        return this->default_device_period;
    }
}

int Device::getBufferSizeIn(size_units_t units) {
    switch(units) {
    case size_units_t::BYTES:
        return this->buffer_size_bytes;
    case size_units_t::FRAMES:
        return this->buffer_size_frames;
    case size_units_t::SAMPLES:
        return this->buffer_size_frames * this->channel_count;
    default:
        return this->buffer_size_frames;
    }
}

int Device::getPaddedBufferSizeIn(size_units_t units) {
    switch(units) {
    case size_units_t::BYTES:
        Logger::errCynth("TODO: Padded buffer size in bytes not implemented.");
    case size_units_t::FRAMES:
        return this->padded_buffer_size_frames;
    case size_units_t::SAMPLES:
        return this->padded_buffer_size_frames / this->channel_count;
    default:
        return this->padded_buffer_size_frames;
    }
}

int Device::getPaddedBufferSize() {
    return this->getPaddedBufferSizeIn(size_units_t::FRAMES);
}

int Device::getChannelCount() {
    return this->channel_count;
}

int Device::getBitDepth() {
    return this->bit_depth;
}

int Device::getSampleRate() {
    return this->sample_rate;
}

void Device::waitForBuffer() {
    this->audio_client.waitForBuffer();
}

byte_t* Device::getBuffer() {
    UINT32 padding = 0;
    HRESULT hr = this->audio_client->GetCurrentPadding(&padding);
    if (FAILED(hr))
        Logger::errHRESULT(hr);
    this->padded_buffer_size_frames = this->buffer_size_frames - padding;

    byte_t* ptr_buffer;
    hr = this->audio_client.render_client->GetBuffer(
        this->padded_buffer_size_frames,
        &ptr_buffer);
    if (FAILED(hr))
        Logger::errHRESULT(hr);
    
    return ptr_buffer;
}

void Device::releaseBuffer() {
    DWORD flags = 0; // TODO
    HRESULT hr = this->audio_client.render_client->ReleaseBuffer(
        this->padded_buffer_size_frames, flags);
    if (FAILED(hr))
        Logger::errHRESULT(hr);
}

void Device::start() {
    this->audio_client.start();
}

void Device::stop() {
    this->audio_client.stop();
}