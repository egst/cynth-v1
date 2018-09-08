#pragma once

/* Platform setup: */
#include "platform.hpp"

/* WASAPI interaction: */

/* Tools: */
#include "tools.hpp"
//#include "config.hpp" // TODO
#include "logger.hpp"

/* Environments: */
#ifdef CYNTH_ENV_CLI
//#include "env_cli/cli.hpp"
//#include "env_cli/script.hpp" // TODO
#endif

/* Audio APIs setup: */
#ifdef CYNTH_API_WASAPI
//#include "api/wasapi/setup.hpp"
#endif

/* Synthesizer devices abstraction: */
#include "user_library/devices/hub.hpp"
#include "user_library/devices/lfo.hpp"
#include "user_library/devices/sequencer.hpp"
#include "user_library/devices/sound_card.hpp"
#include "user_library/devices/tone_generator.hpp"
#include "user_library/functions/types.hpp"
#include "user_library/functions/wave_function.hpp"
#include "user_library/functions/wave_functions.hpp"
#include "user_library/functions/sequence_function.hpp"
#include "user_library/functions/sequence_element.hpp"