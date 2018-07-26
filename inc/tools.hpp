#pragma once

#include <string>

namespace Cynth {
    /* Implementation tools: */
    namespace Tools {

        /* Comparing floats: */
        bool compare(float a, float b);

        /* wchar conversion: */
        std::string wcharToString(wchar_t* input);
    }
}