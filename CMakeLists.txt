cmake_minimum_required(VERSION 3.5)
project(Cynth CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-Wall")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(SOURCES
    ${PROJECT_SOURCE_DIR}/src/api/wasapi/audio_client.cpp
    ${PROJECT_SOURCE_DIR}/src/api/wasapi/setup.cpp
    ${PROJECT_SOURCE_DIR}/src/api/wasapi/device_collection.cpp
    ${PROJECT_SOURCE_DIR}/src/api/wasapi/device.cpp
    ${PROJECT_SOURCE_DIR}/src/api/wasapi/device_enumerator.cpp
    ${PROJECT_SOURCE_DIR}/src/api/wasapi/interface.cpp
    ${PROJECT_SOURCE_DIR}/src/api/wasapi/property_store.cpp
    ${PROJECT_SOURCE_DIR}/src/api/wasapi/render_client.cpp
    ${PROJECT_SOURCE_DIR}/src/pcm/functions/conversion_function.cpp
    ${PROJECT_SOURCE_DIR}/src/pcm/functions/conversion_functions.cpp
    ${PROJECT_SOURCE_DIR}/src/pcm/functions/time_function.cpp
    ${PROJECT_SOURCE_DIR}/src/pcm/buffer.cpp
    ${PROJECT_SOURCE_DIR}/src/pcm/sample.cpp
    ${PROJECT_SOURCE_DIR}/src/rack/components/receiver_port.cpp
    ${PROJECT_SOURCE_DIR}/src/rack/components/transmitter_port.cpp
    ${PROJECT_SOURCE_DIR}/src/rack/devices/input_device.cpp
    ${PROJECT_SOURCE_DIR}/src/rack/devices/midi_device.cpp
    ${PROJECT_SOURCE_DIR}/src/rack/devices/oscillator.cpp
    ${PROJECT_SOURCE_DIR}/src/rack/devices/output_device.cpp
    ${PROJECT_SOURCE_DIR}/src/user_library/devices/hub.cpp
    ${PROJECT_SOURCE_DIR}/src/user_library/devices/lfo.cpp
    ${PROJECT_SOURCE_DIR}/src/user_library/devices/sequencer.cpp
    ${PROJECT_SOURCE_DIR}/src/user_library/devices/sound_card.cpp
    ${PROJECT_SOURCE_DIR}/src/user_library/devices/tone_generator.cpp
    ${PROJECT_SOURCE_DIR}/src/user_library/functions/sequence_element.cpp
    ${PROJECT_SOURCE_DIR}/src/user_library/functions/sequence_function.cpp
    ${PROJECT_SOURCE_DIR}/src/user_library/functions/wave_function.cpp
    ${PROJECT_SOURCE_DIR}/src/user_library/functions/wave_functions.cpp
    ${PROJECT_SOURCE_DIR}/src/entry.cpp
    ${PROJECT_SOURCE_DIR}/src/logger.cpp
    ${PROJECT_SOURCE_DIR}/src/tools.cpp)

include_directories(${PROJECT_SOURCE_DIR}/inc)
add_executable(Cynth ${SOURCES})