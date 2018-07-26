/* An implementation file for: */
#include "setup.hpp"

/* Local libraries: */
// Platform dependent macros:
#include "platform.hpp"
// Windows WASAPI:
#include "WASAPI.hpp"

/* Standard libraries: */
#include <vector>
#include <string>
#include <iostream>

Cynth::Setup::Setup(CYNTH_API audio_API)
    : audio_API(audio_API) {
    
    // TODO
    
    std::vector<std::string> device_names;
    device_names = audio_API.getRenderingDevicesNames();
    std::cout << "Choose output devices from:" << std::endl;
    for(int i = 0; i < device_names.size(); i++) {
        std::cout << '\t' << i << ": " << device_names[i] << std::endl;
    }
    std::cout << std::endl;
    std::cout << "Number of channels: ";
    int input;
    std::vector<int> devices_selected;
    std::cin >> input;
    switch(input) {
    case 0:
        std::cout << "Choose 1 or more devices.";
        break;
    case 1:
        std::cout << "Device: ";
        std::cin >> input;
        devices_selected.push_back(input);
        break;
    case 2:
        std::cout << "Left: ";
        std::cin >> input;
        devices_selected.push_back(input);
        std::cout << std::endl;
        std::cout << "Right: ";
        std::cin >> input;
        devices_selected.push_back(input);
    default:
        std::cout << "Surround is not supported yet." << std::endl;
    }
}