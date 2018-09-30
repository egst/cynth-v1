#pragma once

/* Local libraries: */
#include "pcm/sample.hpp"

/* Standard libraries: */
#include <vector>
#include <limits>

namespace Cynth::PCM {

    class Buffer {
    private:
        std::vector<unsigned char> bytes;
        int bit_depth;

    public:
        Buffer(int bit_depth);

        void write(Sample sample);
        // TODO:
        Buffer& operator<<(Sample sample);
        unsigned char* data();
        std::size_t size();
        void clear();
        void copyTo(
            unsigned char* ptr_buffer,
            std::size_t size = std::numeric_limits<std::size_t>::max());
        void moveTo(
            unsigned char* ptr_buffer,
            std::size_t size = std::numeric_limits<std::size_t>::max());
    };

}