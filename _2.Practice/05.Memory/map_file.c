#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <windows.h>

static void mapping(const char *filename) {
    HANDLE h_file = CreateFile(filename, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
    if (h_file != INVALID_HANDLE_VALUE) {
        DWORD file_size = GetFileSize(h_file, 0);
        HANDLE h_mapped = CreateFileMapping(h_file, 0, PAGE_READONLY, 0, 0, 0);
        if (h_mapped != INVALID_HANDLE_VALUE) {
            uint8_t *mapped_memory = MapViewOfFile(h_mapped, FILE_MAP_READ, 0, 0, file_size);
            if (mapped_memory != 0) {
                fprintf(stdout, "Mapped: 0x%p\n", (void *)mapped_memory);
                UnmapViewOfFile(mapped_memory);
            }
            CloseHandle(h_mapped);
        }
        CloseHandle(h_file);
    }
}


int main(int argc, char **argv) {
    fprintf(stdout, "Mapping %s\n", argv[0]);
    mapping(argv[0]);
    return EXIT_SUCCESS;
}

