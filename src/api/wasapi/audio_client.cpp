/* An implementation file for: */
#include "api/wasapi/audio_client.hpp"

/* Local libraries: */
#include "logger.hpp"

using namespace Cynth::API::WASAPI;
using Cynth::Logger;

void AudioClient::setEventHandle() {
    this->event_buffer = NULL;
    this->event_buffer = CreateEventA(NULL, FALSE, FALSE, NULL);
    if (this->event_buffer == NULL)
        Logger::errWASAPI("Creating an event failed.");
    
    HRESULT hr = this->ptr_instance->SetEventHandle(this->event_buffer);
    if (FAILED(hr))
        Logger::errHRESULT(hr);
}

void AudioClient::waitForBuffer() {
    // Wait for the buffer, or 2 seconds if the event doesn't trigger:
    DWORD result = WaitForSingleObject(this->event_buffer, 2000);
    if (result != WAIT_OBJECT_0) {
        this->stop();
        Logger::errWASAPI("Buffer event did not trigger.");
    }
}

void AudioClient::start() {
    HRESULT hr = this->ptr_instance->Start();
    if (FAILED(hr))
        Logger::errHRESULT(hr);
}

void AudioClient::stop() {
    HRESULT hr = this->ptr_instance->Stop();
    if (FAILED(hr))
        Logger::errHRESULT(hr);
}