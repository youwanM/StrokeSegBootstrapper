#pragma once
/**
* @file CommandLineTools.h
* @brief Header file for the CommandLineTools module.
* This file contains declarations for functions related to command line argument extraction,
* Python executable path checking, and command line preparation for executing Python scripts.
* @author Florent LERAY
* @date 2023-10-01
* @version 1.0
* @copyright Copyright (c) 2023 INRIA. All rights reserved.
*/
#include "deps.h"

#define PYTHON_BIN_NAME   L"python.exe"
#define APP_RELATIVE_PATH L".\\stroke_seg.py"

/**
 * @brief Extracts command line arguments and checks the Python executable path.
 * @param [OUT] argc Reference to the argument count.
 * @param [OUT] currentExecutableDirPath Reference to the current executable directory path.
 * @param [OUT] pythonExecutablePath Reference to the Python executable path.
 * @return A pointer to an array of wide strings (LPWSTR) containing the command line arguments, equivalent to a `argvW`.
 */
LPWSTR* extractAndCheck(int& argc, std::filesystem::path& currentExecutableDirPath, std::filesystem::path& pythonExecutablePath);

/**
* @brief Prepares the command line for execution.
* @param scriptFileName The name of the script file to execute.
* @param pythonExecutablePath The path to the Python executable.
* @param argvW Pointer to an array of wide strings (LPWSTR) containing the command line arguments.
* @param argc The number of command line arguments.
* @param console Reference to a boolean indicating if the console should be used.
* @return A wide string (std::wstring) containing the prepared command line.
*/
std::wstring commandLineConverter(std::wstring scriptFileName, std::filesystem::path pythonExecutablePath, LPWSTR* argvW, int argc, bool& console);
