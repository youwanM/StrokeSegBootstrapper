#include "helpers.h"

// Helper function to convert char* (ANSI/UTF-8) to std::wstring (UTF-16)
std::wstring ConvertToWideString(const char* str)
{
    if (str == nullptr)
    {
        return std::wstring();
    }

    // Get the required buffer size for the wide string
    int size_needed = MultiByteToWideChar(CP_UTF8, 0, str, -1, NULL, 0);
    if (size_needed == 0)
    {
        // Handle conversion error, e.g., return empty string or throw
        return std::wstring();
    }

    // Create a wstring with the required size
    std::wstring wstr(size_needed - 1, 0); // -1 to exclude the null terminator for std::wstring

    // Perform the conversion
    MultiByteToWideChar(CP_UTF8, 0, str, -1, &wstr[0], size_needed);

    return wstr;
}