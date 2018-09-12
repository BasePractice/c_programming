#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#pragma section(".unknown", read)
__declspec(allocate(".unknown")) const int unk_variable = 0x11223344;

__declspec(code_seg(".func"))
static int segment_function() {
    return 0x11223344;
}

int main() {
    uint8_t stack_memory[1000] = {0};
    uint8_t *dynamic_memory = 0;

    fprintf(stdout, "Section    : 0x%p\n", (void *) &unk_variable);
    fprintf(stdout, "Stack      : 0x%p\n", (void *) stack_memory);
    dynamic_memory = calloc(1000, 1);
    fprintf(stdout, "Dyn[calloc]: 0x%p\n", (void *) dynamic_memory);
    free(dynamic_memory);
    dynamic_memory = malloc(1000);
    fprintf(stdout, "Dyn[malloc]: 0x%p\n", (void *) dynamic_memory);
    free(dynamic_memory);
    return segment_function();
}