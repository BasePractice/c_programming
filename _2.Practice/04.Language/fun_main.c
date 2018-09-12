#include <stdio.h>
#include <stdlib.h>

#if defined(_MSC_VER)
#define stdcall __stdcall
#define cdecl   __cdecl
#else
#define stdcall __attribute__((stdcall))
#define cdecl   __attribute__((cdecl))
#endif

void stdcall fun_stdcall() {
    fprintf(stdout, "fun_stdcall\n");
    fflush(stdout);
}

void cdecl fun_cdecl() {
    fprintf(stdout, "fun_cdecl\n");
    fflush(stdout);
}

int main(int argc, char **argv) {
    fun_cdecl();
    fun_stdcall();
#if defined(_MSC_VER)
#else
    void *c_address = 0;
    asm("call .int_cdecl;\n\t"
        ".int_cdecl: \n\t"
        "pop %%rax;\n\t"
        "movq %%rax, %0;" : "=r"(c_address));
    fprintf(stdout, "Address: 0x%p\n", c_address);
#endif
    return EXIT_SUCCESS;
}
