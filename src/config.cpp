/* An implementation file for: */
#include "config.hpp"

/* Standard libraries: */
#include <fstream>

/* -- Config: --------------------------------------------------------------- */

Cynth::Config::Config(std::string file_name)
    : file_name(file_name) {
    std::ifstream input(this->file_name);
    std::string key;
    std::string val;
    this->content.clear();
    while (getline(input, key, ':')) {
        getline(input, val, '\n');
        this->content.insert(std::pair<std::string, std::string>(key, val));
    }
}

template<>
std::string Cynth::Config::get<std::string>(std::string key) {
    return this->content[key];
}

template<>
bool Cynth::Config::get<bool>(std::string key) {
    std::string str_val;
    bool val;
    str_val = Cynth::Config::get<std::string>(key);
    if (str_val == "0"
    ||  str_val == "false"
    ||  str_val == "off")
        val = false;
    else
    if (str_val == "1"
    ||  str_val == "true"
    ||  str_val == "on")
        val = true;
    return val;
};

template<>
int Cynth::Config::get<int>(std::string key) {
    return std::stoi(Cynth::Config::get<std::string>(key));
};

template<>
float Cynth::Config::get<float>(std::string key) {
    return std::stof(Cynth::Config::get<std::string>(key));
};

void Cynth::Config::set(std::string key, std::string val) {
    this->content[key] = val;
}

void Cynth::Config::set(std::string key, bool val) {
    if (val)
        this->set(key, "true");
    else
        this->set(key, "false");
};

void Cynth::Config::set(std::string key, int val) {
    Cynth::Config::set(key, std::to_string(val));
};

void Cynth::Config::set(std::string key, float val) {
    Cynth::Config::set(key, std::to_string(val));
};