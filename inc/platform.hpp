#pragma once

/* Local libraries: */
// Windows WASAPI:
#include "api/wasapi/control.hpp"

/* Manual setup:

Edit to match required platform. */

// Operating system:
#define CYNTH_OS_WINDOWS
// Environment:
//#define CYNTH_ENV_CLI // Command-line Interface
#define CYNTH_ENV_LIB_TESTING // Library testing
// Audio API:
#define CYNTH_API_WASAPI

/* Automatic setup:

Do not edit. The results are based on the manual setup above. */

// Use this macro to check if the requested platform is supported:
#define CYNTH_PLATFORM_IMPLEMENTED true

#ifdef CYNTH_OS_WINDOWS
// ...
#else
#undef CYNTH_PLATFORM_IMPLEMENTED
#define CYNTH_PLATFORM_IMPLEMENTED false
#endif

#ifdef CYNTH_ENV_CLI
// ...
#else
#undef CYNTH_PLATFORM_IMPLEMENTED
#define CYNTH_PLATFORM_IMPLEMENTED false
#endif

#ifdef CYNTH_API_WASAPI
#define CYNTH_API Cynth::API::WASAPI::Control
#else
#undef CYNTH_PLATFORM_IMPLEMENTED
#define CYNTH_PLATFORM_IMPLEMENTED false
#endif