#include "json_node_pool/json_node_pool.h"
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main(int argcounter, char* argvalue[]) {
    if (argcounter < 2) {
        fprintf(stderr, "Filename is not provided.");
        exit(-1);
    }

    char* filename = argvalue[1];
    FILE* file = fopen(filename, "r");

    if (file == NULL) {
        fprintf(stderr,"Cannot open the file %s", filename);
        exit(2);
    }

    JSON_node_pool* pool = jsnd_pool_create();
    jsnd_pool_read_from_file(pool, file);
    
}
