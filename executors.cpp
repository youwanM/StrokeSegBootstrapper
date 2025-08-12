#include "executors.h"

bool execWithConsole(const std::wstring& childExePath, const std::wstring& selfDirPath, int consoleStatus)
{
    bool bRes = true;

    STARTUPINFOW si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    // Get the standard input, output, and error handles of the parent process
    HANDLE hStdIn = GetStdHandle(STD_INPUT_HANDLE);
    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    HANDLE hStdErr = GetStdHandle(STD_ERROR_HANDLE);

    // Inherit the standard handles so that the child process can use them
    SetHandleInformation(hStdIn, HANDLE_FLAG_INHERIT, HANDLE_FLAG_INHERIT);
    SetHandleInformation(hStdOut, HANDLE_FLAG_INHERIT, HANDLE_FLAG_INHERIT);
    SetHandleInformation(hStdErr, HANDLE_FLAG_INHERIT, HANDLE_FLAG_INHERIT);

    // Specify to STARTUPINFO to use these handles
    si.dwFlags |= STARTF_USESTDHANDLES;
    si.hStdInput = hStdIn;
    si.hStdOutput = hStdOut;
    si.hStdError = hStdErr;

    // Command line to execute the child process. CreateProcessW requires a writable wide string (wchar_t*).
    std::wstring commandLine = childExePath;
    
    // Launch the child process using CreateProcessW for wide characters
    std::wcout << L"Application starting.\n\rPlease wait...\n\r" << std::endl;
    if (!CreateProcessW(
        nullptr,          // No module name (use command line)
        &commandLine[0],  // Command line
        nullptr,          // Process handle not inheritable
        nullptr,          // Thread handle not inheritable
        TRUE,             // Set handle inheritance to TRUE
        0,                // No creation flags (important: NOT CREATE_NEW_CONSOLE)
        nullptr,          // Use parent's environment block
        &selfDirPath[0],  // Use parent's starting directory
        &si,              // Pointer to STARTUPINFO structure
        &pi               // Pointer to PROCESS_INFORMATION structure
    ))
    {
        std::wcerr << L"CreateProcessW failed (" << GetLastError() << L")." << std::endl;
        bRes = false;
    }

    // Wait for the child process to finish
    WaitForSingleObject(pi.hProcess, INFINITE);

    // Close process and thread handles
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    
    if (consoleStatus == 0) // If consoleStatus is 0, we assume a new console was created
    {
        std::wcout << L"Child process finished.\n\rPress any key to exit console..." << std::endl;
        std::cin.ignore();
    }
    else if (consoleStatus == 1) // If consoleStatus is 1, we assume we are inheriting the console from the parent
    {
        std::wcout << L"Finish\r\n" << std::endl;
    }
    FreeConsole();

    return bRes;
}

bool execWitoutConsole(const std::wstring& childExePath, const std::wstring& selfDirPath)
{
    bool bRes = true;

    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    // Explicitly set dwFlags and wShowWindow for GUI applications
    si.dwFlags = STARTF_USESHOWWINDOW;
    si.wShowWindow = SW_HIDE; // This will prevent the console window from appearing

    // Command line to execute the child process. CreateProcessW requires a writable wide string (wchar_t*).
    std::wstring commandLine = childExePath; 

    // Launch the child process (python.exe) using CreateProcessW for wide characters
    if (!CreateProcessW(
        NULL,             // No module name (use command line)
        &commandLine[0],  // Command line arguments (includes python.exe path)
        NULL,             // Process handle not inheritable
        NULL,             // Thread handle not inheritable
        FALSE,            // Set handle inheritance to FALSE
        CREATE_NO_WINDOW, // IMPORTANT: This flag prevents a new console window
        NULL,             // Use parent's environment block
        &selfDirPath[0],  // Use current executable's directory as starting directory
        &si,              // Pointer to STARTUPINFO structure
        &pi               // Pointer to PROCESS_INFORMATION structure
    ))
    {
        MessageBoxW(NULL, (L"Failed to launch Python script. Error code: " + std::to_wstring(GetLastError())).c_str(), L"Launcher Error", MB_OK | MB_ICONERROR);
        bRes = false;
    }

    return bRes;
}
