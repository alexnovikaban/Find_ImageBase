#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>

DWORD ac_client_addr = 0x0;

DWORD GetBaseAddressOfModule(DWORD procId, const wchar_t* modName) {
    DWORD modBaseAddr = 0;

    HANDLE snappi = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, procId);

    if (snappi == INVALID_HANDLE_VALUE) {
        CloseHandle(snappi);
        return modBaseAddr;
    }

    MODULEENTRY32 modEntry;
    modEntry.dwSize = sizeof(modEntry);

    if (Module32First(snappi, &modEntry)) {
        do {
            if (_wcsicmp(modEntry.szModule, modName) == 0) {
                modBaseAddr = (uintptr_t)modEntry.modBaseAddr;
                break;
            }
        } while (Module32Next(snappi, &modEntry));
    }

    CloseHandle(snappi);
    return modBaseAddr;
}

void PrintHackMenu(DWORD proc_id, DWORD imageBase) {
    printf("AssaultCube - External\n");
    printf("======================\n");
    printf("Process id: %d\n", proc_id);
    printf("ImageBase:  0x%x\n\n", imageBase);
    printf("[NUMPAD 9] - Exit\n\n");
}

int main() {
    HWND ac_window = FindWindow(0, L"AssaultCube");
    DWORD proc_id = 0;
    GetWindowThreadProcessId(ac_window, &proc_id);
    HANDLE  hProcess = OpenProcess(PROCESS_ALL_ACCESS, 0, proc_id);
    ac_client_addr = GetBaseAddressOfModule(proc_id, L"ac_client.exe");

    PrintHackMenu(proc_id, ac_client_addr);

    while (true) {
        // Close External
        if (GetAsyncKeyState(VK_NUMPAD9) & 1) {
            break;
        }

        Sleep(10);
    }

    CloseHandle(hProcess);
}