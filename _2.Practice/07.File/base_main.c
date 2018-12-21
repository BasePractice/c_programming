#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <property.h>

#if defined(WIN32)
#include <direct.h>
#define getcwd _getcwd
#else

#include <unistd.h>

#endif

typedef bool (*READ_LINE_CALLBACK)(char *line, void *user_data);

char *get_absolute_filename(const char *filename);
bool is_file_exists(char *filename);
bool is_executable(char *filename);


int main(int argc, char **argv) {
    char *absolute_filename;
    int i;

    if (argc < 2) {
        fprintf(stderr, "File name not defined\n");
        return EXIT_FAILURE;
    }
    for (i = 1; i < argc; ++i) {
        absolute_filename = get_absolute_filename(argv[i]);
        bool exists = is_file_exists(absolute_filename);
        if (!exists) {
            absolute_filename = strdup(argv[i]);
            exists = is_file_exists(absolute_filename);
        }
        bool executable = is_executable(absolute_filename);
        fprintf(stdout, "%s: %s, %s\n", absolute_filename,
                exists ? "exists" : "not found",
                executable ? "executable" : "not executable");
        if (!executable) {
            struct Property *root = property_new("root", "root");
            read_lines(absolute_filename, parse_line, root);
            property_destroy(root, property_dynamic_free, 0);
        }
        free(absolute_filename);
    }
#if defined(WIN32)
    system("pause");
#endif
    return EXIT_SUCCESS;
}

bool is_executable(char *filename) {
    bool executable = false;
    FILE *fd = fopen(filename, "rb");
    if (fd != 0) {
        unsigned char magic[8];
        size_t read;


        read = fread(magic, sizeof(magic), 1, fd);
        if (read == 1) {
#if defined(WIN32)
            executable = magic[0] == 'M' && magic[1] == 'Z';
#else
            executable = magic[1] == 'E'
                         && magic[2] == 'L'
                         && magic[3] == 'F';
#endif
        }
        fclose(fd);
    }
    return executable;
}



bool is_file_exists(char *filename) {
    FILE *fd = fopen(filename, "r");
    if (fd == 0)
        return false;
    fclose(fd);
    return true;
}

char *get_absolute_filename(const char *filename) {
    static char cwd[256] = {0};
    char *absolute_filename;

    if (cwd[0] == 0) {
        getcwd(cwd, sizeof(cwd));
    }
    absolute_filename = (char *) calloc(strlen(cwd) + strlen(filename) + 2, 1);
    strcpy(absolute_filename, cwd);
    strcat(absolute_filename, "/");
    strcat(absolute_filename, filename);
    return absolute_filename;
}
