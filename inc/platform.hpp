#pragma once

// Operating system:
#if !( defined(CYNTH_OS_WINDOWS) || defined(CYNTH_OS_LINUX) )
    #define CYNTH_OS_WINDOWS // (by default)
    // Other options:
    // CYNTH_OS_LINUX - (to be implemented)
#endif

// Environment:
#if !( defined (CYNTH_ENV_LIB) || defined(CYNTH_ENV_CLI) )
    #define CYNTH_ENV_LIB // Library (by default)
    // Other options:
    // CYNTH_ENV_CLI - Command-line Interface (to be implemented)
#endif

// Library modes:
#ifdef CYNTH_ENV_LIB
    #if !( defined(CYNTH_LIB_BUILD) || defined(CYNTH_LIB_USE) || defined(CYNTH_LIB_PY) || defined(CYNTH_LIB_STATIC) )
        #define CYNTH_LIB_USE // To import interfaces (by default)
        // Other options:
        // CYNTH_LIB_BUILD - To export interfaces
    #endif
    // Additional options:
    // CYNTH_LIB_TESTING - Library testing in entry.cpp
#endif

// Audio API:
#if !( defined(CYNTH_API_WASAPI) || defined(CYNTH_API_ALSA) || defined(CYNTH_API_ASIO) )
    #define CYNTH_API_WASAPI // For Windows (by default)
    // Other options:
    // CYNTH_API_ALSA - For Linux (to be implemented)
    // CYNTH_API_ASIO - For Windows (to be implemented)
#endif

// Use this macro to check if the requested platform combination is supported:
#define CYNTH_PLATFORM_IMPLEMENTED true

// Checking unsuported platform combinations:
#if !( defined(CYNTH_OS_WINDOWS) )
    #undef CYNTH_PLATFORM_IMPLEMENTED
    #define CYNTH_PLATFORM_IMPLEMENTED false
#endif
#if !( defined(CYNTH_ENV_LIB) )
    #undef CYNTH_PLATFORM_IMPLEMENTED
    #define CYNTH_PLATFORM_IMPLEMENTED false
#endif
#if !( defined(CYNTH_API_WASAPI) )
    #undef CYNTH_PLATFORM_IMPLEMENTED
    #define CYNTH_PLATFORM_IMPLEMENTED false
#endif

// Define API namespace:
#if defined(CYNTH_API_WASAPI)
    #define CYNTH_API_NS Cynth::API::WASAPI
#elif defined(CYNTH_API_ALSA)
    #define CYNTH_API_NS Cynth::API::ALSA
#elif defined(CYNTH_API_ASIO)
    #define CYNTH_API_NS Cynth::API::ASIO
#endif

// Define declspec keywords for DLL:
#define CYNTH_EXPORT
#if defined(CYNTH_ENV_LIB) && defined(CYNTH_OS_WINDOWS)
    #if defined(CYNTH_LIB_BUILD)
        #undef CYNTH_EXPORT
        #define CYNTH_EXPORT __declspec(dllexport)
    #elif defined(CYNTH_LIB_USE)
        #undef CYNTH_EXPORT
        #define CYNTH_EXPORT __declspec(dllimport)
    #endif
#endif