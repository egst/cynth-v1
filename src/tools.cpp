/* An implementation file for: */
#include "tools.hpp"

/* Standard libraries: */
#include <cmath>
#include <limits>

bool Cynth::Tools::compare(float a, float b) {
    return std::fabs(a - b) < std::numeric_limits<float>::epsilon();
}

bool Cynth::Tools::compare(double a, double b) {
    return std::fabs(a - b) < std::numeric_limits<double>::epsilon();
}

std::string Cynth::Tools::wcharToString(wchar_t* input) {;
    std::wstring wstr(input);
    std::string output(wstr.begin(), wstr.end());
    return output;
    // TODO: Delete the original pointer;
}

#ifdef CYNTH_OS_WINDOWS
std::string Cynth::Tools::guidToString(GUID guid) {
    char guid_cstr[39];
    snprintf(
        guid_cstr,
        sizeof(guid_cstr),
        "{%08lx-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x}",
        guid.Data1,
        guid.Data2,
        guid.Data3,
        guid.Data4[0],
        guid.Data4[1],
        guid.Data4[2],
        guid.Data4[3],
        guid.Data4[4],
        guid.Data4[5],
        guid.Data4[6],
        guid.Data4[7]);
	return std::string(guid_cstr);
}
#endif