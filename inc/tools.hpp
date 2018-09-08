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
        bool compare(double a, double b);
        bool compare(float a, double b);
        bool compare(double a, float b);
        bool compare(float a, int b);
        bool compare(int a, float b);
        bool compare(double a, int b);
        bool compare(int a, double b);

        /* wchar conversion: */
        std::string wcharToString(wchar_t* input);

        /* GUID: */
        #ifdef CYNTH_OS_WINDOWS
        std::string guidToString(GUID guid);
        #endif
    }
}