/* An implementation file for: */
#include "api/wasapi/interface.hpp"

/* Local libraries: */
#include "logger.hpp"

using namespace Cynth::API::WASAPI;
using Cynth::Logger;

template<typename interface_t>
Interface<interface_t>::Interface()
    : ptr_instance(nullptr), auto_release(true) {}

template<typename interface_t>
Interface<interface_t>::Interface(interface_t* ptr_instance)
    : ptr_instance(ptr_instance), auto_release(true) {}

template<typename interface_t>
Interface<interface_t>::~Interface() {
    if (this->auto_release && this->ptr_instance) {
        this->ptr_instance->Release();
        this->ptr_instance = nullptr;
    }
}

template<typename interface_t>
void Interface<interface_t>::autoRelease() {
    this->auto_release = true;
}

template<typename interface_t>
void Interface<interface_t>::noAutoRelease() {
    this->auto_release = false;
}

template<typename interface_t>
interface_t* Interface<interface_t>::operator->() {
    return this->ptr_instance;
}

template<typename interface_t>
Interface<interface_t>::Interface(const Interface<interface_t>& other) {
    Logger::errCynth("An interface may not be copied.");
}

template<typename interface_t>
Interface<interface_t>&
Interface<interface_t>::operator=(const Interface<interface_t>& other) {
    Logger::errCynth("An interface may not be copied.");
    return *this;
}

template<typename interface_t>
Interface<interface_t>::Interface(Interface<interface_t>&& other) = default;

template<typename interface_t>
Interface<interface_t>& Interface<interface_t>::operator=(
    Interface<interface_t>&& other) = default;

/* Windows libraries: */
// Audio Client:
//#include <audioclient.h>
// Multimedia Device API:
//#include <mmdeviceapi.h>

/* Types: */
template class Interface<IAudioClient>;
template class Interface<IMMDeviceCollection>;
template class Interface<IMMDeviceEnumerator>;
template class Interface<IMMDevice>;
template class Interface<IPropertyStore>;
template class Interface<IAudioRenderClient>;