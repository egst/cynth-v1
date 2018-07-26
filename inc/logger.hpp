#pragma once

/* Component Object Model: */
#include <objbase.h> // -> HRESULT

namespace Cynth {

    class Logger {
    private:
        // General case exit of program:
        static void exit();

    public:
        // Handle HRESULT error:
        static void hresultErr(HRESULT hr);

        // Log HRESULT:
        static void hresultLog(HRESULT hr);
    };

}