#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <windows.h>
#include <tlhelp32.h>

#define PROCESS_NAME "05.Memory_process_memory.exe"
#define WRITE_TEXT   "This text write in another process"

static DWORD find_process(const char *process_name) {
    HANDLE h_search;
    DWORD pid = 0L;

    h_search = CreateToolhelp32Snapshot(TH32CS_SNAPALL, 0);
    if (h_search != INVALID_HANDLE_VALUE) {
        PROCESSENTRY32 pe32;

        pe32.dwSize = sizeof(pe32);
        if (Process32First(h_search, &pe32) == TRUE) {
            do {
                if (stricmp(pe32.szExeFile, process_name) == 0) {
                    pid = pe32.th32ProcessID;
                    break;
                }
            } while (Process32Next(h_search, &pe32) == TRUE);
        }
        CloseHandle(h_search);
    }
    return pid;
}

static void do_process(DWORD pid) {
    HANDLE h_process = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
    if (h_process != NULL && h_process != INVALID_HANDLE_VALUE) {
        size_t memory_size = strlen(WRITE_TEXT);
        void *process_memory = VirtualAllocEx(h_process, 0, memory_size, MEM_COMMIT, PAGE_READWRITE);
        if (process_memory != NULL) {
            int ret = WriteProcessMemory(h_process, process_memory, WRITE_TEXT, memory_size, NULL);
            fprintf(stdout, "Memory write %s\n", ret == TRUE ? "success" : "failure");

            /*
            Inject DLL
            PTHREAD_START_ROUTINE ll = (PTHREAD_START_ROUTINE)
                    GetProcAddress(GetModuleHandle("kernel32"), "LoadLibraryA");
            HANDLE h_remote = CreateRemoteThread(h_process, NULL, 0, ll, process_memory, 0, NULL);
            WaitForSingleObject(h_remote, INFINITE);
            */

            VirtualFreeEx(h_process, 0, memory_size, MEM_RELEASE);
        }
        CloseHandle(h_process);
    }
}

int main(int argc, char **argv) {
    DWORD pid = find_process(PROCESS_NAME);
    if (pid > 0) {
        fprintf(stdout, "Process %lu\n", pid);
        do_process(pid);
    } else {
        fprintf(stdout, "Process \"%s\" not found\n", PROCESS_NAME);
    }
    return EXIT_SUCCESS;
}

