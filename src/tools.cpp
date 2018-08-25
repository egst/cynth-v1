/* An implementation file for: */
#include "tools.hpp"

#include <cmath>  // -> std::fabs()
#include <limits> // -> std::numeric_limits::epsilon()

bool Cynth::Tools::compare(float a, float b) {
    return std::fabs(a - b) < std::numeric_limits<float>::epsilon();
}

std::string Cynth::Tools::wcharToString(wchar_t* input) {;
    std::wstring wstr(input);
    std::string output(wstr.begin(), wstr.end());
    return output;
    // TODO: Delete the original pointer;
}