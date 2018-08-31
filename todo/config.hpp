#pragma once

/* Standard libraries: */
#include <string>
#include <map>

namespace Cynth {

    class Config {
    private:
        std::string file_name;
        std::map<std::string, std::string> content;
    
    public:
        /* Cnstructor: */
        Config(std::string file_name);

        /* Accessors: */
        template <typename val_t>
        val_t get(std::string key);

        /* Mutators: */
        void set(std::string key, std::string val);
        void set(std::string key, bool val);
        void set(std::string key, int val);
        void set(std::string key, float val);
    };

}