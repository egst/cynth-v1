#pragma once

/* Local libraries: */
// Windows WASAPI:
#include "WASAPI.hpp"

/* Manual setup:

Edit to match required platform. */

// Operating system:
#define CYNTH_OS_WINDOWS
// Environment:
#define CYNTH_ENV_CLI // Command-line Interface
// Audio API:
#define CYNTH_API_WASAPI

/* Automatic setup:

Do not edit. The results are based on the manual setup above. */

// Use this macro to check if the requested platform is supported:
#define CYNTH_PLATFORM_IMPLEMENTED true

#ifdef CYNTH_OS_WINDOWS
// ...
#else
#define CYNTH_PLATFORM_IMPLEMENTED false
#endif

#ifdef CYNTH_ENV_CLI
// ...
#else
#define CYNTH_PLATFORM_IMPLEMENTED false
#endif

#ifdef CYNTH_API_WASAPI
#define CYNTH_API Cynth::WASAPI::Control
#else
#define CYNTH_PLATFORM_IMPLEMENTED false
#endif