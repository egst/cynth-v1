/* An implementation file for: */
#include "pcm/buffer.hpp"

/* Local libraries: */
#include "logger.hpp"
#include "pcm/sample.hpp"

/* Standard libraries: */
#include <cstring>

using namespace Cynth::PCM;
using Cynth::Logger;

Buffer::Buffer(int bit_depth): bit_depth(bit_depth) {
    if (bit_depth % 8 != 0)
        Logger::errCynth("Wrong bit depth.");
}

void Buffer::write(Sample sample) {
    for(int i = 0; i < this->bit_depth / 8; i++) {
        this->bytes.push_back(sample[i]);
    }
}

unsigned char* Buffer::data() {
    return this->bytes.data();
}

std::size_t Buffer::size() {
    return this->bytes.size();
}

void Buffer::clear() {
    this->bytes.clear();
}

void Buffer::copyTo(unsigned char* ptr_buffer) {
    std::memcpy(ptr_buffer, this->data(), this->size());
}

void Buffer::moveTo(unsigned char* ptr_buffer) {
    this->copyTo(ptr_buffer);
    this->clear();
}