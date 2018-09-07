#pragma once

/* Local libraries: */
#include "platform.hpp"

/* Standard libraries: */
#include <string>

#ifdef CYNTH_OS_WINDOWS
/* Windows libraries: */
// Component Object Model:
#include <combaseapi.h>
#endif

namespace Cynth {
    /* Implementation tools: */
    namespace Tools {

        /* Comparing floats: */
        bool compare(float a, float b);
        /* Comparing doubles: */
        bool compare(double a, double b);

        /* wchar conversion: */
        std::string wcharToString(wchar_t* input);

        /* GUID: */
        #ifdef CYNTH_OS_WINDOWS
        std::string guidToString(GUID guid);
        #endif
    }
}