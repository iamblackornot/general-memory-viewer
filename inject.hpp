#pragma once

#include <windows.h>
#include <tlhelp32.h>
#include <iostream>

inline DWORD GetProcessIdByName(const wchar_t* processName) 
{
    PROCESSENTRY32W entry;
    entry.dwSize = sizeof(PROCESSENTRY32W);

    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (Process32FirstW(snapshot, &entry)) 
    {
        while (Process32NextW(snapshot, &entry)) 
        {
            if (_wcsicmp(entry.szExeFile, processName) == 0) 
            {
                CloseHandle(snapshot);
                return entry.th32ProcessID;
            }
        }
    }
    CloseHandle(snapshot);
    return 0;
}

inline bool ReadProcessMemoryEx(DWORD pid, LPCVOID address, LPVOID buffer, SIZE_T size) 
{
    HANDLE hProcess = OpenProcess(PROCESS_VM_READ, FALSE, pid);
    if (hProcess == NULL) 
    {
        std::cerr << "Failed to open process. Error: " << GetLastError() << std::endl;
        return false;
    }

    SIZE_T bytesRead;
    if (!ReadProcessMemory(hProcess, address, buffer, size, &bytesRead)) 
    {
        std::cerr << "Failed to read memory. Error: " << GetLastError() << std::endl;
        CloseHandle(hProcess);
        return false;
    }

    CloseHandle(hProcess);
    return true;
}