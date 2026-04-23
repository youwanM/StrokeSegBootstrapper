#include "executors.h"

bool execWithConsole(const std::wstring& childExePath, const std::wstring& selfDirPath, int consoleStatus)
{
    bool bRes = true;

    STARTUPINFOW si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    HANDLE hStdIn = GetStdHandle(STD_INPUT_HANDLE);
    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    HANDLE hStdErr = GetStdHandle(STD_ERROR_HANDLE);

    SetHandleInformation(hStdIn, HANDLE_FLAG_INHERIT, HANDLE_FLAG_INHERIT);
    SetHandleInformation(hStdOut, HANDLE_FLAG_INHERIT, HANDLE_FLAG_INHERIT);
    SetHandleInformation(hStdErr, HANDLE_FLAG_INHERIT, HANDLE_FLAG_INHERIT);

    si.dwFlags |= STARTF_USESTDHANDLES;
    si.hStdInput = hStdIn;
    si.hStdOutput = hStdOut;
    si.hStdError = hStdErr;

    std::wstring commandLine = childExePath;

    if (!CreateProcessW(
        nullptr,
        &commandLine[0],
        nullptr,
        nullptr,
        TRUE,
        0,
        nullptr,
        &selfDirPath[0],
        &si,
        &pi
    ))
    {
        std::wcerr << L"CreateProcessW failed (" << GetLastError() << L")." << std::endl;
        bRes = false;
    }
    else
    {
        WaitForSingleObject(pi.hProcess, INFINITE);
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
    }

    if (consoleStatus == 0)
    {
        std::wcout << L"\nChild process finished.\n\rPress any key to exit console..." << std::endl;
        std::cin.ignore();
    }
    else if (consoleStatus == 1)
    {
        std::wcout << L"\nFinish\r\n" << std::endl;
    }
    FreeConsole();

    return bRes;
}

bool execWitoutConsole(const std::wstring& childExePath, const std::wstring& selfDirPath)
{
    bool bRes = true;

    STARTUPINFOW si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    si.dwFlags = STARTF_USESHOWWINDOW;
    si.wShowWindow = SW_SHOW;

    std::wstring commandLine = childExePath;

    if (!CreateProcessW(
        NULL,
        &commandLine[0],
        NULL,
        NULL,
        FALSE,
        CREATE_NO_WINDOW,
        NULL,
        &selfDirPath[0],
        &si,
        &pi
    ))
    {
        MessageBoxW(NULL, (L"Failed to launch Python GUI. Error code: " + std::to_wstring(GetLastError())).c_str(), L"Launcher Error", MB_OK | MB_ICONERROR);
        bRes = false;
    }
    else
    {
        WaitForSingleObject(pi.hProcess, INFINITE);
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
    }

    return bRes;
}