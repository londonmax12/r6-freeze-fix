#include <windows.h>
#include <iostream>
#include <tlhelp32.h>

HANDLE GetProcessHandleByName(const wchar_t* processName)
{
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnapshot == INVALID_HANDLE_VALUE) {
        return nullptr;
    }

    PROCESSENTRY32 pe32;
    pe32.dwSize = sizeof(PROCESSENTRY32);

    if (!Process32First(hSnapshot, &pe32)) {
        CloseHandle(hSnapshot);
        return nullptr;
    }

    do {
        if (_wcsicmp(pe32.szExeFile, processName) == 0) {
            HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pe32.th32ProcessID);
            CloseHandle(hSnapshot);
            return hProcess;
        }
    } while (Process32Next(hSnapshot, &pe32));

    CloseHandle(hSnapshot);
    return nullptr;
}

int main() {
    printf("--------------------------------\n\n");
    printf("\tR6 Freeze Fix\n");
    printf("Made by Mercury#1337\n\n");
    printf("--------------------------------\n\n");

    DWORD_PTR processMask, systemMask;
    HANDLE hProcess = GetProcessHandleByName(L"RainbowSix.exe");
    if (!hProcess)
    {
        printf("R6 is currently not running!\n");
        system("PAUSE");
        return 0;
    }
    printf("R6 process found\n");
    GetProcessAffinityMask(hProcess, &processMask, &systemMask);
    if (processMask == 1) {
        printf("R6 is only running on 1 CPU cores, it is recommended to run the game with at least 4 cores\n");
        system("PAUSE");
        return 0;
    }
    if (processMask == 0xF) {
        printf("R6 running on all CPU cores\n");
    }
    else {
        int coreCount = 0;
        for (DWORD_PTR mask = processMask; mask; mask >>= 1) {
            coreCount += (mask & 1);
        }
        printf("R6 running on %i cores\n", coreCount);
    }
    
    printf("Setting affinty to 1\n");
    SetProcessAffinityMask(hProcess, 1);

    printf("Set affinity to prvious value\n");
    SetProcessAffinityMask(hProcess, processMask);
    
    printf("\nThe game should be fixed now, enjoy!\n");
    system("PAUSE");
    return 0;
}