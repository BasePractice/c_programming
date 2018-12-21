#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <property.h>

#define PROPERTY_FILE_NAME "/home/pastor/git/c_programming/_2.Practice/.temporary/settings.properties"

struct Property *root = 0;

static void mun_delete(const char *key) {
    if (root == 0)
        return;
    property_delete(root, key);
}

static void mun_update(char *key, char *value) {
    if (root == 0)
        return;
    property_update(root, strdup(key), strdup(value));
}

static void mun_list(struct Property **list) {
    if (root == 0)
        return;
    (*list) = root;
}

static void mun_init(char *filename) {
    if (root != 0)
        return;
    root = property_new("root", "root");
    read_lines(filename, parse_line, root);
}


static void print_properties(struct Property *list) {
    fprintf(stdout, "################\n");
    property_it(list, property_print, 0);
}


int
main(int argc, char **argv) {
    //Инициализация МУН
    struct Property *list = 0;
    mun_init(PROPERTY_FILE_NAME);

    mun_list(&list);
    {
        print_properties(list);
        mun_update("new_property", "89");
        mun_list(&list);
        print_properties(list);
        mun_update("new_property", "80");
        mun_list(&list);
        print_properties(list);
    }
    //Удаление МУН
    property_destroy(root, property_dynamic_free, 0);
    return EXIT_SUCCESS;
}
