#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "json_tokens_handler/json_tokens_handler.h"

int main(int argcounter, char* argvalue[]) {
    if (argcounter < 2) {
        fprintf(stderr, "Filename is not provided.");
        exit(-1);
    }

    char* filename = argvalue[1];
    FILE* file = fopen(filename, "r");

    if (file == NULL) {
        fprintf(stderr, "Cannot open the file %s", filename);
        exit(2);
    }

    printf("\nReading...");
    JSON_node* root = jstkn_read_from_file(file);
    printf("\nPrinting...");
    jsnd_print_nodes_reqursively(root);
}
