#pragma once

/* Platform setup: */
#include "platform.hpp"

/* WASAPI interaction: */

/* Tools: */
#include "tools.hpp"
#include "config.hpp"
#include "logger.hpp"

/* Environments: */
#ifdef CYNTH_ENV_CLI
#include "env_cli/cli.hpp"
#include "env_cli/script.hpp"
#endif

/* Audio APIs: */
#ifdef CYNTH_API_WASAPI
#include "api/wasapi.hpp"
#endif

/* Synthesizer devices abstraction: */
#include "rack/functions.hpp"
#include "rack/devices.hpp"
#include "rack/oscillators.hpp"