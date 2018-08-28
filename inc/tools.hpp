#pragma once

#include "platform.hpp"

#include <string>

namespace Cynth {
    /* Implementation tools: */
    namespace Tools {

        /* Comparing floats: */
        bool compare(float a, float b);

        /* wchar conversion: */
        std::string wcharToString(wchar_t* input);

        /* GUID: */
        #ifdef CYNTH_OS_WINDOWS
        std::string guidToString(GUID guid);
        #endif
    }
}