#include "deps.h"
#include "commandLineTools.h"

LPWSTR* extractAndCheck(int& argc, std::filesystem::path& currentExecutableDirPath, std::filesystem::path& pythonExecutablePath)
{
    LPWSTR* argvW = nullptr;
    LPWSTR lpCmdLineW = GetCommandLineW();
    argvW = CommandLineToArgvW(lpCmdLineW, &argc);

    if (argvW)
    {
        if (argc >= 1)
        {
            wchar_t currentExecutablePathBuf[MAX_PATH];
            GetModuleFileNameW(NULL, currentExecutablePathBuf, MAX_PATH);

            currentExecutableDirPath = std::filesystem::path(currentExecutablePathBuf).parent_path();
            // Path to embedded Python
            pythonExecutablePath = currentExecutableDirPath / L".." / L"Python" / PYTHON_BIN_NAME;

            if (!std::filesystem::exists(pythonExecutablePath))
            {
                MessageBoxW(NULL, (L"Error: python.exe not found at " + pythonExecutablePath.wstring()).c_str(), L"Launcher Error", MB_OK | MB_ICONERROR);
                LocalFree(argvW);
                return nullptr;
            }
        }
        else
        {
            MessageBoxW(NULL, L"Error: No command line arguments provided.", L"Launcher Error", MB_OK | MB_ICONERROR);
            LocalFree(argvW);
            return nullptr;
        }
    }
    return argvW;
}

std::wstring commandLineConverter(std::wstring scriptFileName, std::filesystem::path pythonExecutablePath, LPWSTR* argvW, int argc, bool& console)
{
    std::wstringstream commandLineStream;

    // Get the absolute path to the Python script
    wchar_t buf[MAX_PATH];
    GetModuleFileNameW(NULL, buf, MAX_PATH);
    std::filesystem::path scriptPath = std::filesystem::path(buf).parent_path() / scriptFileName;

    // 1. Quoted path to Python
    commandLineStream << L"\"" << pythonExecutablePath.wstring() << L"\"";

    // 2. Quoted absolute path to the script
    commandLineStream << L" \"" << scriptPath.wstring() << L"\"";

    // Default to GUI mode
    console = false;

    // 3. Blindly forward all arguments to Python
    for (int i = 1; i < argc; ++i)
    {
        std::wstring arg = argvW[i];

        std::wstring arglo = arg;
        std::transform(arglo.begin(), arglo.end(), arglo.begin(), [](auto c) { return towlower(c); });

        if (arglo == L"--console") {
            console = true;
        }

        if (!arg.empty() && arg.back() == L'\\')
        {
            arg += L'\\';
        }

        // Forward the argument quoted
        commandLineStream << L" \"" << arg << L"\"";
    }

    return commandLineStream.str();
}