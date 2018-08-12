#include <stdlib.h>
#include <stdio.h>

#if defined(WIN32)
#include <Windows.h>

#define dlopen LoadLibrary
#define dlsym  GetProcAddress
#else
#include <dlfcn.h>
#endif

#define DLL_LIBRARY_NAME "lib03.System.OS_export_if.dll"
#define DLL_IMPORT_NAME "hello_text"

typedef char * (*Pfn_ExportIf_Function)(void);

Pfn_ExportIf_Function pfn_export_if;

int main(int argc, char **argv) {
    char *hello = 0;
    void *h;

    h = dlopen(DLL_LIBRARY_NAME);
    if (h == 0) {
        fprintf(stderr, "Library: %s not found\n", DLL_LIBRARY_NAME);
        return -1;
    }

    if (argc > 1) {
        pfn_export_if = (void *)strtoll(argv[1], 0, 16);
    } else {
        pfn_export_if = (Pfn_ExportIf_Function) dlsym(h, DLL_IMPORT_NAME);
    }
    fprintf(stdout, "Symbol: 0x%p\n", pfn_export_if);
    if (pfn_export_if == 0) {
        if (pfn_export_if == 0) {
            fprintf(stderr, "Symbol: %s not found\n", DLL_IMPORT_NAME);
            return -2;
        }
    }
    hello = (*pfn_export_if)();
    fprintf(stdout, "Text: %s\n", hello);
    return EXIT_SUCCESS;
}


