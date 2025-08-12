#include "deps.h"

int allocateConsole(wchar_t const * consoleTitle)
{
    int iRes = -1; // Default return value indicating failure

    if (AttachConsole(ATTACH_PARENT_PROCESS))
    {
        iRes = 1; // Successfully attached to the parent console
    }
    else if (AllocConsole())
    {
        iRes = 0; // Successfully allocated a new console
        SetConsoleTitle(consoleTitle);
    }

    if (iRes != -1)
    {
        FILE* pCout = nullptr;
        FILE* pCin  = nullptr;
        FILE* pCerr = nullptr;

        freopen_s(&pCout, "CONOUT$", "w", stdout);
        freopen_s(&pCin,  "CONIN$",  "r", stdin);
        freopen_s(&pCerr, "CONOUT$", "w", stderr);

        // Synchronise C++ streams with C stdio
        std::ios_base::sync_with_stdio(true);
        std::cout.clear();
        std::cin.clear();
        std::cerr.clear();
    }
    else
    {
        MessageBox(nullptr, TEXT("Could not allocate or attach console!"), TEXT("Error"), MB_OK | MB_ICONERROR);
    }

    return iRes;
}