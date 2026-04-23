#include "consoleAllocator.h"
#include "executors.h"
#include "commandLineTools.h"

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    int argc = 0;
    std::filesystem::path currentExecutableDirPath;
    std::filesystem::path pythonExecutablePath;

    LPWSTR* argvW = extractAndCheck(argc, currentExecutableDirPath, pythonExecutablePath);

    if (argvW)
    {
        bool console = false;

        // APP_RELATIVE_PATH should be defined in deps.h as L"stroke_seg.py"
        std::wstring commandLine = commandLineConverter(APP_RELATIVE_PATH, pythonExecutablePath, argvW, argc, console);

        if (!console)
        {
            // GUI Phase: Run silently, wait for the user to pick a threshold and close
            execWitoutConsole(commandLine, currentExecutableDirPath.wstring());
        }
        else
        {
            // Console Phase: Allocate console, run the heavy processing
            int consoleStatus = allocateConsole(TEXT("Stroke Segmentation Engine"));
            execWithConsole(commandLine, currentExecutableDirPath.wstring(), consoleStatus);
        }
    }
    else
    {
        return 1;
    }

    LocalFree(argvW);
    return 0;
}