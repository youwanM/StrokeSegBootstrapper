#pragma once
/**
* @file helpers.h
* @brief Header file for helper functions.
* This file contains declarations for various utility functions used in the project.
* It includes functions for string conversion, file operations, and other common tasks.
* @author Florent LERAY
* @date 2023-10-01
* @version 1.0
* @copyright Copyright (c) 2023 INRIA. All rights reserved.
*/
#include "deps.h"

/**
 * @brief Converts a UTF-8 encoded string to a wide string (UTF-16).
 * @param str The UTF-8 encoded string to convert.
 * @return A wide string (UTF-16) representation of the input string.
 */
std::wstring ConvertToWideString(const char* str);
