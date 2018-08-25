/* An implementation file for: */
#include "cli/cli.hpp"

/* Local libraries: */
// Platform dependent macros:
#include "platform.hpp"
// Windows WASAPI:
#include "api/wasapi.hpp"

/* Standard libraries: */
#include <string>
#include <iostream>

Cynth::CLI::CLI(CYNTH_API audio_API)
    : audio_API(audio_API) {
    
    this->rendering_devices_count = this->audio_API.getRenderingDevicesCount();
}

/* -- CLI: ------------------------------------------------------------------ */

#ifdef CYNTH_ENV_CLI

void Cynth::CLI::setup() {
    std::cout << "Choose output devices from:" << std::endl;
    this->printRenderingDevices();
    std::vector<int> devices_selected = this->promptRenderingDevices();
    this->audio_API.setRenderingDevices(devices_selected);
}

void Cynth::CLI::printRenderingDevices() {
    std::vector<std::string> device_names;
    device_names = this->audio_API.getRenderingDevicesNames();
    for(int i = 0; i < device_names.size(); i++) {
        std::cout << i << ": " << device_names[i] << std::endl;
    }
}

std::vector<int> Cynth::CLI::promptRenderingDevices() {
    int input;
    std::vector<int> devices_selected;
    while (true) {
        std::cout << "Enter number of channels: ";
        std::cin >> input;
        if (input < 1) {
            std::cout << "At least one channel required." << std::endl;
            continue;
        }
        if (input > 2) {
            std::cout << "Surround not supported yet." << std::endl;
            continue;
        }
        break;
    }
    switch(input) {
    case 1:
        while (true) {
            std::cout << "Enter device number: ";
            std::cin >> input;
            if (input < 0 || input >= this->rendering_devices_count) {
                std::cout << "Enter device number in the provided range."
                          << std::endl;
                continue;
            }
            break;
        }
        devices_selected.push_back(input);
        break;
    case 2:
        std::cout << "Enter device numbers for:" << std::endl;
        while (true) {
            std::cout << "Left: ";
            std::cin >> input;
            if (input < 0 || input >= this->rendering_devices_count) {
                std::cout << "Enter device number in the provided range."
                          << std::endl;
                continue;
            }
            break;
        }
        devices_selected.push_back(input);
        int left = input;
        while (true) {
            std::cout << "Right: ";
            std::cin >> input;
            if (input == left) {
                std::cout << "Enter a different device for the right channel."
                          << std::endl;
                continue;
            }
            if (input < 0 || input >= this->rendering_devices_count) {
                std::cout << "Enter device number in the provided range."
                          << std::endl;
                continue;
            }
            break;
        }
        devices_selected.push_back(input);
    }

    return devices_selected;
}

#endif