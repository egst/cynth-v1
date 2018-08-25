#pragma once

/* Local libraries: */
#include "platform.hpp"

/* Standard libraries: */
#include <string>

#ifdef CYNTH_ENV_CLI
namespace Cynth {

    class Parser {
    public:
        enum commands_t {
            GET,
            SET,
            NEW,
            CONNECT,
            START,
            RUN
        };
        static void separateCommands();
        static commands_t identifyCommand(std::string command_string);
        template<typename command_t>
        static command_t parseCommand(std::string command_string);
        template<typename command_t>
        static void runCommand(command_t command);
    };

    class Command {
    public:
        /* Execute: */
        virtual void operator()();
    };

    class SetupCommand: public Command {};

    class GetCommand: public Command {
    private:
        /* Properties: */
        // Namespace:
        std::string space;
        // Name:
        std::string name;
        // Value:
        std::string value;
    
    public:
        /* Constructor: */
        GetCommand(std::string name, std::string space);

        /* Execute: */
        void operator()();
    };

    class SetCommand: public Command {};

    class NewCommand: public Command {};

    class ConnectCommand: public Command {};

    class StartCommand: public Command {};

    class RunCommand: public Command {};

}
#endif