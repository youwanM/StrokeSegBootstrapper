#include "consoleAllocator.h"
#include "executors.h"
#include "commandLineTools.h"

/**
 * @mainpage Stroke Segmentation Project bootstrap
 *
 * # Stroke Segmentation Project bootstrap
 *
 * This application is a bootstrap for the Stroke Segmentation, which is an IA Python application designed to segment strokes in images.
 * This bootstrap is an Windows C++ 20 application. It serves as a launcher for the Python script responsible for stroke segmentation in a good **mode GUI, Console or both**.
 * It is designed to be used in a Windows environment and provides a simple way to run the Python script with the necessary command line arguments.
 * 
 * Mains features include:
 * - Extracting command line arguments from the bootstrap application.
 * - Executing the Python script with the provided arguments.
 * - Allocating a console if needed.
 * - Translate the command line arguments for the boot strap to the python application.
 * - Handling the execution of the Python script **with** or **without** a console window.
 * - Ensuring compatibility with the Windows operating system, especially to handle wide char (UTF 16 encoding).
 * - Providing a simple and effective way to run Python scripts from a Windows application.
 *
 * ## Goals
 *
 * - Main gaol is to provide a simple and effective way to run the Stroke Segmentation Python script from a Windows application.
 * - Determine if IA Python application run on mode **console, GUI or both**.
 * - Encapsulate the complexity of setting up the environment for running the script on embedded python.
 *
 * ## How to build it
 *
 * Visual Studio 2022 is required to build this project.
 * Simply build the project in Visual Studio, and it will generate an executable file that can be run on Windows.
 * Use the following steps to build the project:
 * - Open the project in Visual Studio 2022.
 * - Open the solution file `StrokeSegmentationBootstrap.sln`.
 * - Select the build configuration (Debug or Release).
 * - Build the project by clicking on the "Build" button or pressing `Ctrl + Shift + B`.
 * - The output executable will be located in the `bin` directory of the project.
 *
 * ## License
 *
 * This project is under AGPL license.
 */

/**
 * @brief Entry point for the Windows application.
 * @param hInstance Handle to the current instance of the application.
 * @param hPrevInstance Handle to the previous instance of the application (always NULL for Win32 apps).
 * @param lpCmdLine Pointer to the command line string (ANSI).
 * @param nCmdShow Specifies how the main window of the application should be shown.
 * @return Returns the exit code of the application. 1 if an error occurs, 0 otherwise.
 */
int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    int argc = 0;
    std::filesystem::path currentExecutableDirPath;
    std::filesystem::path pythonExecutablePath;

    LPWSTR* argvW = extractAndCheck(argc, currentExecutableDirPath, pythonExecutablePath);

    if (argvW)
    {
        bool console = false;
        std::wstring commandLine = commandLineConverter(APP_RELATIVE_PATH, pythonExecutablePath, argvW, argc, console);

        if (!console)
        {
            execWitoutConsole(commandLine, currentExecutableDirPath.wstring());
        }
        else
        {
            int consoleSatus = allocateConsole(TEXT("Stroke Seg Console"));
            execWithConsole(commandLine, currentExecutableDirPath.wstring(), consoleSatus);
        }
    }
    else
    {        
        return 1; // If argvW is nullptr, an error message has already been displayed in extractAndCheck.
    }
   
    LocalFree(argvW); // Free the memory allocated by CommandLineToArgvW

    return 0;
}
