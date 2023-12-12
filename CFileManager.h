#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#define MAX_FILE_NAME 128
#define MAX_FILE_PATH 256


#ifdef _WIN32
const char SEPARATOR = '\\';
#else
const char SEPARATOR = '/';
#endif