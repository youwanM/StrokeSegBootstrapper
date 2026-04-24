#include "consoleAllocator.h"
#include "executors.h"
#include "commandLineTools.h"

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    int argc = 0;
    std::filesystem::path currentExecutableDirPath;
    std::filesystem::path pythonExecutablePath;

    // This will hold the final status of our application
    DWORD finalStatus = 1;

    LPWSTR* argvW = extractAndCheck(argc, currentExecutableDirPath, pythonExecutablePath);

    if (argvW)
    {
        bool console = false;

        // APP_RELATIVE_PATH should be defined in deps.h as L"stroke_seg.py"
        std::wstring commandLine = commandLineConverter(APP_RELATIVE_PATH, pythonExecutablePath, argvW, argc, console);

        if (!console)
        {
            // GUI Phase
            finalStatus = execWitoutConsole(commandLine, currentExecutableDirPath.wstring());
        }
        else
        {
            // Console Phase
            int consoleStatus = allocateConsole(TEXT("Stroke Segmentation Engine"));
            finalStatus = execWithConsole(commandLine, currentExecutableDirPath.wstring(), consoleStatus);
        }
    }
    else
    {
        return 1;
    }

    LocalFree(argvW);

    // Return the status back to syngo.via!
    return static_cast<int>(finalStatus);
}