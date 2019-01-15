#pragma once

#define NOMINMAX //

/* Platform setup: */
#include "platform.hpp"

/* WASAPI interaction: */

/* Tools: */
//#include "config.hpp" // TODO
#include "logger.hpp"

/* Synthesizer devices abstraction: */
#include "user_library/types.hpp"
#include "user_library/devices/envelope.hpp"
#include "user_library/devices/hub.hpp"
#include "user_library/devices/lfo.hpp"
#include "user_library/devices/sequencer.hpp"
#include "user_library/devices/sound_card.hpp"
#include "user_library/devices/tone_generator.hpp"
#include "user_library/functions/wave_function.hpp"
#include "user_library/functions/wave_functions.hpp"
#include "user_library/functions/sequence_function.hpp"
#include "user_library/functions/sequence_element.hpp"