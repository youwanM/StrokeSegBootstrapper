#pragma once
/**
* @file consoleAllocator.h
* @brief Header file for console allocation functions.
* This file contains declarations for functions that allocate a console for the application.
* @author Florent LERAY
* @date 2023-10-01
* @version 1.0
* @copyright Copyright (c) 2023 INRIA. All rights reserved.
*/

/**
* @brief Allocates a console for the application.
* This function creates a new console for the application, allowing it to use standard input/output streams.
* It also redirects the standard input, output, and error streams to the console.
* @return 0 if a new console was successfully allocated, 1 if the console was attached to the parent process, -1 if the console could not be allocated or attached.
*/
int allocateConsole(wchar_t const * consoleTitle = L"");