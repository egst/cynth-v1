/* An implementation file for: */
#include "pcm/sample.hpp"

/* Local libraries: */
#include "logger.hpp"

/* Standard libraries: */
#include <cstring>

using namespace Cynth::PCM;
using Cynth::Logger;

Sample::Sample() {}

Sample::Sample(int bit_depth): bit_depth(bit_depth) {
    if (bit_depth % 8 != 0)
        Logger::errCynth("Wrong bit depth.");
    this->bytes.resize(bit_depth / 8);
}

Sample& Sample::operator=(unsigned long value) {
    std::memcpy(this->bytes.data(), &value, this->bytes.size());
    return *this;
}

Sample& Sample::operator=(long value) {
    value <<= (sizeof(value) - this->bytes.size()) * 8;
    std::memcpy(this->bytes.data(), &value, this->bytes.size());
    return *this;
}

Sample& Sample::operator=(float value) {
    if (sizeof(value) != this->bytes.size())
        Logger::errCynth(
            "Unnable to convert a value to the sample bit depth.");
    std::memcpy(this->bytes.data(), &value, this->bytes.size());
    return *this;
}

Sample& Sample::operator=(double value) {
    if (sizeof(value) != this->bytes.size())
        Logger::errCynth(
            "Unnable to convert a value to the sample bit depth.");
    std::memcpy(this->bytes.data(), &value, this->bytes.size());
    return *this;
}

unsigned char Sample::operator[](int i) {
    return this->bytes[i];
}

unsigned char* Sample::data() {
    return this->bytes.data();
}

std::size_t Sample::size() {
    return this->bytes.size();
}