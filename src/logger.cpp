/* An implementation file for: */
#include "logger.hpp"

/* Standard libraries: */
#if defined(CYNTH_ENV_CLI) \
 || defined(CYNTH_ENV_LIB) \
 || defined(CYNTH_ENV_LIB_TESTING)
#include <iostream>
#endif
#include <sstream>
#include <exception>

/* -- Logger: --------------------------------------------------------------- */

void Cynth::Logger::errHRESULT(HRESULT hr) {
    throw Cynth::COMException(hr);
}

void Cynth::Logger::errPlatform() {
    throw Cynth::PlatformException();
}

void Cynth::Logger::errPlatform(std::string message) {
    throw Cynth::PlatformException(message);
}

void Cynth::Logger::errWASAPI(std::string message) {
    throw Cynth::WASAPIException(message);
}

void Cynth::Logger::errCynth(std::string message) {
    throw Cynth::CynthException(message);
}

void Cynth::Logger::log(std::string message) {
    #if defined(CYNTH_ENV_CLI) \
     || defined(CYNTH_ENV_LIB) \
     || defined(CYNTH_ENV_LIB_TESTING)
    std::cout << message << std::endl;
    #else
    Cynth::Logger::errPlatform(
        "Only CLI and LIB environments supported for log.");
    #endif
}

void Cynth::Logger::log(int message) {
    Cynth::Logger::log(std::to_string(message));
}

void Cynth::Logger::log(float message) {
    Cynth::Logger::log(std::to_string(message));
}

/* -- Exceptions: ----------------------------------------------------------- */

Cynth::Exception::Exception(
    std::string message,
    std::string source) // = "Unknown"
    : std::exception(), message(message), source(source) {}

Cynth::Exception::Exception(
    int message,
    std::string source) // = "Unknown"
    : message(std::to_string(message)), source(source) {}

std::string Cynth::Exception::getMessage() noexcept {
    return this->message;
}

std::string Cynth::Exception::getSource() noexcept {
    return this->source;
}

std::string Cynth::Exception::what() noexcept {
    return "[" + this->source + "] " + this->message;
}

void Cynth::Exception::log() noexcept {
    Cynth::Logger::log(this->what());
}

Cynth::COMException::COMException(HRESULT message)
    : Cynth::Exception::Exception((int) message, "COM") {
    
    std::stringstream stream;
    stream << std::hex << message;
    this->message = stream.str();
}

Cynth::PlatformException::PlatformException(std::string message)
    : Cynth::Exception::Exception(message, "Platform") {}

Cynth::WASAPIException::WASAPIException(std::string message)
    : Cynth::Exception::Exception(message, "WASAPI") {}

Cynth::CynthException::CynthException(std::string message)
    : Cynth::Exception::Exception(message, "Cynth") {}