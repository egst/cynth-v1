/* An implementation file for: */
#include "api/wasapi/audio_client.hpp"

/* Local libraries: */
#include "logger.hpp"

using namespace Cynth::API::WASAPI;
using Cynth::Logger;

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