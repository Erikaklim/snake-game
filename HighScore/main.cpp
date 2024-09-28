#include <windows.h>
#include "HighScore.h"
#include <tchar.h>
#include <cstdio>

#define HIGH_SCORE_FILE _T("highscore.dat")

void __stdcall SaveHighScore(int score) {
    HANDLE hFile = CreateFile(
        HIGH_SCORE_FILE,
        GENERIC_WRITE,
        0,
        NULL,
        CREATE_ALWAYS,
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );

    if (hFile == INVALID_HANDLE_VALUE) {
        DWORD error = GetLastError();
        TCHAR errorMsg[256];
        FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, error, 0, errorMsg, 256, NULL);
        _ftprintf(stderr, _T("CreateFile failed: %s\n"), errorMsg);
        return;
    }

    DWORD bytesWritten;
    BOOL result = WriteFile(
        hFile,
        &score,
        sizeof(int),
        &bytesWritten,
        NULL
    );

    if (!result) {
        DWORD error = GetLastError();
        TCHAR errorMsg[256];
        FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, error, 0, errorMsg, 256, NULL);
        _ftprintf(stderr, _T("WriteFile failed: %s\n"), errorMsg);
    }

    CloseHandle(hFile);
}

int __stdcall LoadHighScore(void) {
    int score = 0;
    HANDLE hFile = CreateFile(
        HIGH_SCORE_FILE,
        GENERIC_READ,
        0,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );

    if (hFile == INVALID_HANDLE_VALUE) {
        DWORD error = GetLastError();
        TCHAR errorMsg[256];
        FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, error, 0, errorMsg, 256, NULL);
        _ftprintf(stderr, _T("CreateFile failed: %s\n"), errorMsg);
        return score;
    }

    DWORD bytesRead;
    BOOL result = ReadFile(
        hFile,
        &score,
        sizeof(int),
        &bytesRead,
        NULL
    );

    if (!result) {
        DWORD error = GetLastError();
        TCHAR errorMsg[256];
        FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, error, 0, errorMsg, 256, NULL);
        _ftprintf(stderr, _T("ReadFile failed: %s\n"), errorMsg);
    }

    CloseHandle(hFile);
    return score;
}

extern "C" BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
    switch (ul_reason_for_call) {
        case DLL_PROCESS_ATTACH:
            break;
        case DLL_THREAD_ATTACH:
        case DLL_THREAD_DETACH:
        case DLL_PROCESS_DETACH:
            break;
    }
    return TRUE;
}
