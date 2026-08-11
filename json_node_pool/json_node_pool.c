#include "json_node_pool.h"

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../json_node/json_node.h"

typedef struct JSON_node_pool {
    JSON_node** roots;
    int length;
} JSON_node_pool;

void raise_error(char* reason, int code) {
    fprintf(stderr, "Error occured: %s, exiting...", reason);
    free(reason);
    exit(code);
}

void raise_error_with_token(char* reason, int token, int code) {
    fprintf(stderr, "Error occured: '%s', on token ==> %c <==, exiting...", reason, token);
    free(reason);
    exit(code);
}

JSON_node_pool* jsnd_pool_create() {
    JSON_node_pool* pool = malloc(sizeof(JSON_node_pool*));
    pool->roots = malloc(sizeof(JSON_node**));
    return pool;
}

void jsnd_pool_read_from_file(JSON_node_pool* pool, FILE* file) {
    char token_double_quote = '\"';
    char token_single_quote = '\'';
    char token_left_curle_brace = '{';
    char token_right_curle_brace = '}';
    char token_left_square_brace = '[';
    char token_right_square_brace = ']';
    char* token_true = "true";
    char* token_false = "false";
    char token_colon = ':';
    char token_comma = ',';
    char token_end_of_line = '\n';

    int token = fgetc(file);
    if (token == EOF) {
        raise_error("File is empty", 7);
    }

    void* value;
    char* string = malloc(sizeof(char*));
    JSON_node* working_node = jsnd_create();

    // first-level loop, just to reach the end of the file. There should be 1 cycle only, unless
    // there is 2 jsons in one file.
    while ((token) != EOF) {
        // entering the file, if it fails, error occurs due to syntax error in the biggining of json
        // file
        if (token != token_left_curle_brace) {
            raise_error_with_token("Cannot read a file, file should begin with \' } \'", token, 9);
        }

        // that is the second-level loop, the main one,
        // which will work until we reach the end of the file
        while ((token = fgetc(file)) != token_right_curle_brace) {
            // begin of the first object in the file
            if ((token = fgetc(file)) == token_left_curle_brace) {
                // checking if after proccessing the object, we have a comma or end of the of the
                // file
                if ((token = fgetc(file)) != token_comma) {
                    raise_error_with_token("Unexpected token, expected \' , \'", token, 5);
                }
            }
        }
    }

    if (working_node == NULL) {
        raise_error_with_token("Root node was not found", token, 6);
    }
}

void jsnd_pool_assign_node(JSON_node_pool* pool, JSON_node* node) {}
