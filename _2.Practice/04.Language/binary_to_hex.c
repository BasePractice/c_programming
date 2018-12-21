#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#include <assert.h>

static const char hex[] = "0123456789ABCDEF";

int
binary_to_hex(const uint8_t *binary, size_t binary_size, char *hex_text, size_t hex_text_size) {
    int i;

    if (binary == 0 || hex_text == 0 || binary_size == 0 || hex_text_size == 0)
        return 0;
    if (binary_size * 2 > hex_text_size)
        return 0;
    memset(hex_text, 0, hex_text_size);
    for (i = 0; i < binary_size; ++i) {
        int k = i * 2;
        hex_text[k] = hex[(binary[i] >> 4) & 0xF];
        hex_text[k + 1] = hex[(binary[i]) & 0xF];
    }
    return 1;
}

int main(int argc, char **argv) {
    FILE *fd = fopen(argv[0], "rb");
    uint8_t *binary;
    size_t binary_size;

    if (fd == 0)
        return EXIT_FAILURE;
    fseek(fd, 0, SEEK_END);
    binary_size = (size_t) ftell(fd);
    rewind(fd);
    binary = malloc(binary_size);
    assert(fread(binary, binary_size, 1, fd) == 1);
    fclose(fd);

    {
        size_t hex_size = (binary_size * 2) + 1;
        char *hex = malloc(hex_size);
        assert(binary_to_hex(binary, binary_size, hex, hex_size) == 1);
        fprintf(stdout, "%s\n", hex);
        free(hex);
    }
    free(binary);
    return EXIT_SUCCESS;
}