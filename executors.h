#pragma once
/**
* @file executors.h
* @brief Header file for the executors module.
* This file contains declarations for functions related to launching child processes
* in the parent console or in the background.
* @author Florent LERAY
* @date 2023-10-01
* @version 1.0
* @copyright Copyright (c) 2023 INRIA. All rights reserved.
*/

#include "deps.h"

/**
 * @brief Launches a child process in the parent console.
 * @details This function creates a child process that inherits the console of the parent process.
 * @param commandLine The command line to execute.
 * @param selfDirPath The directory path of the current executable.
 * @param consoleStatus The status of the console (1 for inherit console from parent, 0 for new fresh console, -1 for no console available).
 * @return True if the process was launched successfully, false otherwise.
 */
DWORD execWithConsole(const std::wstring& commandLine, const std::wstring& selfDirPath, int consoleStatus = 0);

/**
 * @brief Launches a child process in the background.
 * @details This function creates a child process that runs in the background without inheriting the console of the parent process.
 * @param commandLine The command line to execute.
 * @param selfDirPath The directory path of the current executable.
 * @return True if the process was launched successfully, false otherwise.
 */
DWORD execWitoutConsole(const std::wstring& commandLine, const std::wstring& selfDirPath);