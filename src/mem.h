#pragma once
#include <windows.h>
#include <TlHelp32.h>
#include <vector>
#include <string>

namespace ProcUtils {
    uintptr_t procId;
    HANDLE hProc;

    DWORD GetProcessIdByName(const wchar_t* processName) {
        DWORD processId = 0;
        HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
        if (snapshot != INVALID_HANDLE_VALUE) {
            PROCESSENTRY32 processEntry;
            processEntry.dwSize = sizeof(processEntry);
            if (Process32First(snapshot, &processEntry)) {
                do {
                    if (_wcsicmp(processEntry.szExeFile, processName) == 0) {
                        processId = processEntry.th32ProcessID;
                        break;
                    }
                } while (Process32Next(snapshot, &processEntry));
            }
            CloseHandle(snapshot);
        }
        return processId;
    }

    uintptr_t GetModuleBaseAddress(DWORD processId, const wchar_t* moduleName) {
        uintptr_t moduleBaseAddress = 0;
        HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, processId);
        if (snapshot != INVALID_HANDLE_VALUE) {
            MODULEENTRY32 moduleEntry;
            moduleEntry.dwSize = sizeof(moduleEntry);
            if (Module32First(snapshot, &moduleEntry)) {
                do {
                    if (_wcsicmp(moduleEntry.szModule, moduleName) == 0) {
                        moduleBaseAddress = reinterpret_cast<uintptr_t>(moduleEntry.modBaseAddr);
                        break;
                    }
                } while (Module32Next(snapshot, &moduleEntry));
            }
            CloseHandle(snapshot);
        }
        return moduleBaseAddress;
    }
}

namespace MemUtils {
    template <class T>
    T RPM(uintptr_t addr) {
        T buffer;
        ReadProcessMemory(ProcUtils::hProc, (BYTE*)addr, &buffer, sizeof(T), 0);
        return buffer;
    }

    template <class T>
    T RPMChain(uintptr_t addr, std::vector<uintptr_t> offsets) {
        for (size_t i = 0; i < offsets.size(); i++) {
            addr = RPM<uintptr_t>(addr + offsets[i]);
        }
        return (T)addr;
    }

    std::string RPMString(uintptr_t addr) {
        std::string buffer;
        char c;
        do {
            ReadProcessMemory(ProcUtils::hProc, (BYTE*)addr, &c, sizeof(c), 0);
            buffer += c;
            addr += sizeof(c);
        } while (c != '\0');
        return buffer;
    }

    template <class T>
    void WPM(uintptr_t addr, T buffer) {
        WriteProcessMemory(ProcUtils::hProc, (BYTE*)addr, &buffer, sizeof(buffer), 0);
    }
}
