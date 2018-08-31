#pragma once

/* Standard libraries: */
#include <vector>

namespace Cynth::PCM {

    class Sample {
    private:
        std::vector<unsigned char> bytes;
        int bit_depth;
    
    public:
        /* Constructors: */
        Sample();
        Sample(int bit_depth);

        /* Mutators: */
        Sample& operator=(long value); // TODO: Check overflow
        Sample& operator=(unsigned long value);
        Sample& operator=(float value);
        Sample& operator=(double value);

        /* Accessors: */
        unsigned char operator[](int i);
        unsigned char* data();
        std::size_t size();
    };
    
}