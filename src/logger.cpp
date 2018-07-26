/* An implementation file for: */
#include "logger.hpp"

/* Standard libraries: */
#include <iostream>  // -> std::cout
#include <exception> // -> std::exception()


void Cynth::Logger::exit() {
    throw std::exception();
}

void Cynth::Logger::hresultErr(HRESULT hr) {
    std::cout << "ERROR: HRESULT ";
    std::cout << "0x" << std::hex << hr;
    std::cout << std::endl;
    Logger::exit();
}

void Cynth::Logger::hresultLog(HRESULT hr) {
    std::cout << "LOG: HRESULT ";
    std::cout << "0x" << std::hex << hr;
    std::cout << std::endl;
}