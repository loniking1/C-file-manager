#include "CFileManager.h"

VOID CreateDirectoryIfNotExist(const CHAR* path) {
    SHORT error;

    if (!CreateDirectory(path, NULL)) {
        error = GetLastError();
        if (error != ERROR_ALREADY_EXISTS) {
            printf_s("Error creating directory: %s (Error code: %d)\n", path, error);
            ExitProcess(error);
        }
    }
}



VOID RecurseCreateDirectoryIfNotExist(const CHAR* dirname) {
    const CHAR* pPath;
    CHAR* temp;
    SHORT error;

    temp = (CHAR*)malloc(strlen(dirname) + 2);
    if (temp == NULL) {
        error = GetLastError();
        printf_s("Memory allocation failed.\n");
        ExitProcess(error);
    }

    // Skip Windows drive letter.
    if ((pPath = strchr(dirname, ':')) != NULL) {
        pPath++;
    }
    else {
        pPath = dirname;
    }

    while ((pPath = strchr(pPath, SEPARATOR)) != NULL) {

        // Skip empty elements.
        if (pPath != dirname && *(pPath - 1) == SEPARATOR) {
            pPath++;
            continue;
        }

        // Put the path up to this point into a temporary variable to pass to the make directory function.
        memcpy(temp, dirname, pPath - dirname);
        temp[pPath - dirname] = '\0';

        pPath++;

        CreateDirectoryIfNotExist(temp);
    }
    free(temp);

    CreateDirectoryIfNotExist(dirname);
}





VOID WriteToFile(const CHAR* filePath, const TCHAR* content) {
    HANDLE hFile;
    INT nBytesWritten;
    SHORT error;

    hFile = CreateFile(filePath, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE) {
        error = GetLastError();
        printf_s("Error creating file: %s (Error code: %d)\n", filePath, error);
        ExitProcess(error);
    }

    if (!WriteFile(hFile, content, strlen(content) * sizeof(TCHAR), &nBytesWritten, NULL)) {
        error = GetLastError();
        printf_s("Error writing to file: %s (Error code: %d)\n", filePath, error);
        CloseHandle(hFile);
        ExitProcess(error);
    }

    CloseHandle(hFile);
}

VOID PrintFileContent(const TCHAR* filePath) {
    HANDLE hFile;
    INT nBytesRead;
    TCHAR* buffer;
    LONG lFileSize;
    SHORT error;

    hFile = CreateFile(filePath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE) {
        error = GetLastError();
        printf_s("Error opening file: %s (Error code: %d)\n", filePath, error);
        ExitProcess(error);
    }

    lFileSize = GetFileSize(hFile, NULL);

    buffer = (TCHAR*)malloc((lFileSize + 1) * sizeof(TCHAR));

    if (buffer == NULL) {
        error = GetLastError();
        printf_s("Memory allocation failed (Error code: %d).\n", error);
        CloseHandle(hFile);
        ExitProcess(error);
    }

    if (!ReadFile(hFile, buffer, lFileSize, &nBytesRead, NULL)) {
        error = GetLastError();
        printf_s("Error reading from file: %s (Error code: %d)\n", filePath, error);
        free(buffer);
        CloseHandle(hFile);
        ExitProcess(error);
    }

    buffer[nBytesRead / sizeof(TCHAR)] = '\0';

    printf_s("\n\nFile Content:\n\n%s\n\n", buffer);

    free(buffer);

    CloseHandle(hFile);
}

VOID FileManager() {
    CHAR* fileName;
    CHAR* filePath;
    TCHAR* content;
    CHAR* completeFilePath;
    SHORT sContentSize;
    SHORT sFilePathLen;
    CHAR cHelper;
    INT error;

    fileName = (CHAR*)malloc(sizeof(CHAR) * MAX_FILE_NAME);
    if (fileName == NULL) {
        error = GetLastError();
        printf_s("Memory allocation failed (Error code: %d).\n", error);
        ExitProcess(error);
    }

    filePath = (CHAR*)malloc(sizeof(CHAR) * MAX_FILE_PATH);
    if (filePath == NULL) {
        error = GetLastError();
        printf_s("Memory allocation failed (Error code: %d).\n", error);
        free(fileName);
        ExitProcess(error);
    }
    
        printf_s("Enter the file name: ");
        if (scanf_s("%s", fileName, MAX_FILE_NAME) != 1) {
            printf_s("Invalid input. Please try again.\n");
            return;
        }

        printf_s("Enter the file path: ");
        if (scanf_s("%s", filePath, MAX_FILE_PATH) != 1) {
            printf_s("Invalid input. Please try again.\n");
            free(fileName);
            return;
        }


        sFilePathLen = strlen(filePath) + strlen(fileName) + 2;

        completeFilePath = (CHAR*)malloc(sFilePathLen * sizeof(CHAR) + 1);

        if (completeFilePath == NULL) {
            error = GetLastError();
            printf_s("Memory allocation failed (Error code: %d).\n", error);
            free(fileName);
            free(filePath);
            ExitProcess(error);
        }

        sprintf_s(completeFilePath, sFilePathLen + 1, "%s\\%s", filePath, fileName);

        //file existance checker
        if (GetFileAttributes(completeFilePath) != INVALID_FILE_ATTRIBUTES) {
            printf_s("File already exists. Do you want to override it? (Y/N): ");
            while (getchar() != '\n'); // clears buffer

            if (scanf_s("%c", &cHelper, 2) != 1) {
                printf_s("Invalid input. Please try again.\n");
                free(fileName);
                free(filePath);
                free(completeFilePath);
                return;
            }

            if (cHelper != 'Y' && cHelper != 'y') {
                printf_s("Okay goodbye!\n");
                free(fileName);
                free(filePath);
                free(completeFilePath);
                return;
            }
        }

        RecurseCreateDirectoryIfNotExist(filePath);

        printf_s("Enter content (EOL to end):\n");

        content = (TCHAR*)malloc(sizeof(TCHAR));
        if (content == NULL) {
            error = GetLastError();
            printf_s("Memory allocation failed (Error code: %d).\n", error);
            free(fileName);
            free(filePath);
            free(completeFilePath);
            ExitProcess(error);
        }

        while (getchar() != '\n'); //clears buffer

        //input handling and writing to file on-input
        sContentSize = 0;
        while ((cHelper = getch()) != 13) {
            content[sContentSize++] = cHelper;

            content = (TCHAR*)realloc(content, (sContentSize + 1) * sizeof(TCHAR));
            if (content == NULL) {
                error = GetLastError();
                printf_s(("Memory reallocation failed (Error code: %d).\n"), error);
                free(fileName);
                free(filePath);
                free(completeFilePath);
                free(content);
                ExitProcess(error);
            }

            content[sContentSize] = ('\0');

            printf_s("%c", cHelper);

            WriteToFile(completeFilePath, content);
        }

        content[sContentSize] = ('\0');

        WriteToFile(completeFilePath, content);

        PrintFileContent(completeFilePath);

        free(fileName);
        free(filePath);
        free(completeFilePath);
        free(content);
}

int main() {
    
    while (1) {
        FileManager();
    }

    return 0;
}
