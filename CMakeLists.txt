cmake_minimum_required(VERSION 3.12)

## Choose one of these OS options: ##
set(CYNTH_OS "WIN")
#set(CYNTH_OS "LINUX") # To be implemented

## Choose one of these audio API options: ##
set(CYNTH_API "WASAPI") # Only for Windows
#set(CYNTH_API "ASIO")   # Only for Windows; To be implemented
#set(CYNTH_API "ALSA")   # Only for Linux; To be implemented

## Choose one of these environment options: ##
#set(CYNTH_ENV "SHARED") # Shared library (Outputs DLL for Windows and SO for Linux)
#set(CYNTH_ENV "PY")     # Python module (Outputs PYD for Windows)
set(CYNTH_ENV "CLI")    # Command line interface; To be implemented
# For static library: Just define the CYNTH_LIB_STATIC macro in your code. (TODO!)

#[[ Note:
    Unimplemented options are marked here as "(To be implemented)"
    and incompatibility of options is always mentioned.
    But to be sure, CYNTH_PLATFORM_IMPLEMENTED macro defined in inc/platform.hpp
    is used to check whether the selected platform options combination is implemented.
]]#

## Project name: ##
project(Cynth CXX)

## C++ Standard: ##
set(CMAKE_CXX_STANDARD 17)

include(${PROJECT_SOURCE_DIR}/cynth.cmake)
include(${PROJECT_SOURCE_DIR}/thirdparty.cmake)

## Warnings: ##
#add_compile_options(-Wall)
#add_compile_options(-Wno-c++98-c++11-c++14-compat)
add_compile_options(-w)

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
	add_compile_options(-fsized-deallocation)
endif()

## Output directory: ##
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CYNTH_DIR_SYSTEM})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CYNTH_DIR_SYSTEM})

## For Python module: ##
if(CYNTH_ENV STREQUAL "PY")
	add_subdirectory(${3RDPARTY_PYBIND11})
    include_directories(${PYBIND11_INCLUDE_DIR}) # Headers for pybind11
    include_directories(${Python3_INCLUDE_DIRS}) # Headers for Python
    #set(PYBIND11_CPP_STANDARD -std=c++1z) # Experimental support of C++17
endif()

## Cynth headers: ##
include_directories(${PROJECT_SOURCE_DIR}/inc)

# Cynth sources:
set(SOURCES
    # WASAPI interfaces abstractions:
    ${PROJECT_SOURCE_DIR}/src/api/wasapi/audio_client.cpp
    ${PROJECT_SOURCE_DIR}/src/api/wasapi/setup.cpp
    ${PROJECT_SOURCE_DIR}/src/api/wasapi/device_collection.cpp
    ${PROJECT_SOURCE_DIR}/src/api/wasapi/device.cpp
    ${PROJECT_SOURCE_DIR}/src/api/wasapi/device_enumerator.cpp
    ${PROJECT_SOURCE_DIR}/src/api/wasapi/interface.cpp
    ${PROJECT_SOURCE_DIR}/src/api/wasapi/property_store.cpp
    ${PROJECT_SOURCE_DIR}/src/api/wasapi/render_client.cpp
    # Wave function abstractions:
    ${PROJECT_SOURCE_DIR}/src/pcm/functions/conversion_function.cpp
    ${PROJECT_SOURCE_DIR}/src/pcm/functions/conversion_functions.cpp
    ${PROJECT_SOURCE_DIR}/src/pcm/functions/time_function.cpp
    # PCM related abstractions:
    ${PROJECT_SOURCE_DIR}/src/pcm/buffer.cpp
    ${PROJECT_SOURCE_DIR}/src/pcm/sample.cpp
    # Devices abstractions:
    ${PROJECT_SOURCE_DIR}/src/rack/components/receiver_port.cpp
    ${PROJECT_SOURCE_DIR}/src/rack/components/transmitter_port.cpp
    ${PROJECT_SOURCE_DIR}/src/rack/devices/input_device.cpp
    ${PROJECT_SOURCE_DIR}/src/rack/devices/midi_device.cpp
    ${PROJECT_SOURCE_DIR}/src/rack/devices/oscillator.cpp
    ${PROJECT_SOURCE_DIR}/src/rack/devices/output_device.cpp
    # User accessible abstractions:
    ${PROJECT_SOURCE_DIR}/src/user_library/devices/envelope.cpp
    ${PROJECT_SOURCE_DIR}/src/user_library/devices/hub.cpp
    ${PROJECT_SOURCE_DIR}/src/user_library/devices/lfo.cpp
    ${PROJECT_SOURCE_DIR}/src/user_library/devices/sequencer.cpp
    ${PROJECT_SOURCE_DIR}/src/user_library/devices/sound_card.cpp
    ${PROJECT_SOURCE_DIR}/src/user_library/devices/tone_generator.cpp
    ${PROJECT_SOURCE_DIR}/src/user_library/functions/sequence_element.cpp
    ${PROJECT_SOURCE_DIR}/src/user_library/functions/sequence_function.cpp
    ${PROJECT_SOURCE_DIR}/src/user_library/functions/wave_function.cpp
    ${PROJECT_SOURCE_DIR}/src/user_library/functions/wave_functions.cpp
    ${PROJECT_SOURCE_DIR}/src/user_library/types.cpp
    # Entry point for CLI and testing:
    ${PROJECT_SOURCE_DIR}/src/entry.cpp
    # Exceptions and log:
    ${PROJECT_SOURCE_DIR}/src/logger.cpp
    # Misc. tools:
    ${PROJECT_SOURCE_DIR}/src/tools.cpp
)

if(CYNTH_ENV STREQUAL "PY")
    # pybind11 module export:
    list(APPEND SOURCES ${PROJECT_SOURCE_DIR}/src/pybind11_module.cpp)
endif()

### Platform setup: ###
## OS: ##
if(CYNTH_OS STREQUAL "WIN")
    add_definitions(-DCYNTH_OS_WINDOWS)
elseif(CYNTH_OS STREQUAL "LINUX")
    add_definitions(-DCYNTH_OS_LINUX)    
endif()
## Environment: ##
if(CYNTH_ENV STREQUAL "SHARED")
    add_definitions(-DCYNTH_ENV_LIB -DCYNTH_LIB_BUILD)
elseif(CYNTH_ENV STREQUAL "PY")
    add_definitions(-DCYNTH_ENV_LIB -DCYNTH_LIB_PY)
elseif(CYNTH_ENV STREQUAL "CLI")
    add_compile_definitions(CYNTH_ENV_CLI)
endif()
## Audio API: ##
if(CYNTH_API STREQUAL "WASAPI")
    add_definitions(-DCYNTH_API_WASAPI)
elseif(CYNTH_API STREQUAL "ALSA")
    add_definitions(-DCYNTH_API_ALSA)
elseif(CYNTH_API STREQUAL "ASIO")
    add_definitions(-DCYNTH_API_ASIO)
endif()

### Build: ###
# Shared library:
if(CYNTH_ENV STREQUAL "SHARED")
    add_library(Cynth SHARED ${SOURCES}) # TODO: Rename? (Remove lib prefix)
# Python module:
elseif(CYNTH_ENV STREQUAL "PY")
    pybind11_add_module(Cynth ${SOURCES} NO_EXTRAS)
    #set_target_properties(Cynth PROPERTIES OUTPUT_NAME Cynth) # TODO: Rename
# CLI:
elseif(CYNTH_ENV STREQUAL "CLI")
    add_executable(Cynth ${SOURCES})
endif()