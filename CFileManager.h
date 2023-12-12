#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#define MAX_FILE_NAME 128
#define MAX_FILE_PATH 256


#ifdef _WIN32
const CHAR SEPARATOR = '\\';
#else
const CHAR SEPARATOR = '/';
#endif

VOID CreateDirectoryIfNotExist(const CHAR* path);
VOID RecurseCreateDirectoryIfNotExist(const CHAR* dirname);
VOID WriteToFile(const CHAR* filePath, const TCHAR* content);
VOID PrintFileContent(const TCHAR* filePath);
VOID FileManager();