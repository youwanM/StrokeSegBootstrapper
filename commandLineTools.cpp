#include "deps.h"
#include "commandLineTools.h"


LPWSTR* extractAndCheck(int& argc, std::filesystem::path& currentExecutableDirPath, std::filesystem::path& pythonExecutablePath)
{
    LPWSTR* argvW = nullptr; // Initialize to nullptr

    LPWSTR lpCmdLineW = GetCommandLineW();

    argvW = CommandLineToArgvW(lpCmdLineW, &argc);

    if (argvW)
    {
        if (argc >= 1)
        {
            // Get the directory of the current executable using wide characters
            wchar_t currentExecutablePathBuf[MAX_PATH];
            GetModuleFileNameW(NULL, currentExecutablePathBuf, MAX_PATH);

            currentExecutableDirPath = std::filesystem::path(currentExecutablePathBuf).parent_path(); // The directory path of the current executable
            pythonExecutablePath = currentExecutableDirPath / L".." / L"Python" / PYTHON_BIN_NAME; // The path to the Python executable

            if (!std::filesystem::exists(pythonExecutablePath))
            {
                MessageBoxW(NULL, (L"Error: python.exe not found at " + pythonExecutablePath.wstring() + L"\nPlease ensure python.exe is in the same directory as this executable.").c_str(), L"Launcher Error", MB_OK | MB_ICONERROR);
                LocalFree(argvW);
                argvW = nullptr;
            }
        }
        else
        {
            MessageBoxW(NULL, L"Error: No command line arguments provided.", L"Launcher Error", MB_OK | MB_ICONERROR);
            LocalFree(argvW);
            argvW = nullptr;
        }
    }

    return argvW;
}

std::wstring commandLineConverter(std::wstring scriptFileName, std::filesystem::path pythonExecutablePath, LPWSTR* argvW, int argc, bool& console)
{
    // Build the command line for the new Python process
    std::wstringstream commandLineStream;

    commandLineStream << L"\"" << pythonExecutablePath.wstring() << L"\""; // The Python executable path itself is the first part of the command line. It must be quoted if it contains spaces.
    commandLineStream << L" \"" << scriptFileName << L"\""; // Enclose in quotes to handle spaces

    bool bOtherArgs = false; // Flag to check if there are any other arguments
    bool bImportModel = false; // Flag to check if there are any other arguments
    bool gui = false;          // Flag to check if GUI mode is needed
    bool verbose = false;      // Flag to check if verbose mode is needed

    for (int i = 1; i < argc; ++i) // Append the rest of the arguments from the original process, starting from argvW[1]
    {
        std::wstring arglo = argvW[i];
        std::transform(arglo.begin(), arglo.end(), arglo.begin(), [](auto c) { return towlower(c); });
        if (arglo == std::wstring(L"--console"))
        {
            console = true;
            continue;
        }
        else if (arglo == std::wstring(L"--verbose"))
        {
            verbose = true; // just to skip the argument from bOtherArgs
        }
        else if (arglo == std::wstring(L"--gui"))
        {
            gui = true; // just to skip the argument from bOtherArgs
        }
        else if (arglo == std::wstring(L"--import-model"))
        {
            bImportModel = true;
        }
        else
        {
            bOtherArgs = true; // If any other argument is found, set the flag to true
        }
        commandLineStream << L" \"" << argvW[i] << L"\"";
    }

    console = console || (bOtherArgs && !gui) || bImportModel; // If there are other arguments, we assume console mode is needed, or if --import-model is specified, we also assume console mode is needed.
    //gui = (gui || !bOtherArgs) && !bImportModel;   // If no other arguments are provided, we assume GUI mode is needed, and if --import-model is specified, we assume GUI mode is not needed.


    return commandLineStream.str();
}
