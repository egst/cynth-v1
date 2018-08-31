#pragma once

/* Local libraries: */
#include "pcm/sample.hpp"

/* Standard libraries: */
#include <vector>

namespace Cynth::PCM {

    class Buffer {
    private:
        std::vector<unsigned char> bytes;
        int bit_depth;

    public:
        Buffer(int bit_depth);

        void write(Sample sample);
        unsigned char* data();
        std::size_t size();
        void clear();
        void copyTo(unsigned char* ptr_buffer);
        void moveTo(unsigned char* ptr_buffer);
    };

}