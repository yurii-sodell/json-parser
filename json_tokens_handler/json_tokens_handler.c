#include "json_tokens_handler.h"
#include "../json_node_pool/json_node_pool.h"
#include "../c-string-builder/string_builder_t.h"

#include <stdlib.h>

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

typedef struct json_tokens_handler
{
   
}json_tokens_handler;



void raise_error(char* reason) {
    fprintf(stderr, "Error occured: %s, exiting...", reason);
    free(reason);
    exit(1);
}

void raise_error_unexpected_token(char expected, char found) {
    fprintf(stderr, "Unexpected token, expected \' %c \', on token ==> %c <==, exiting...",
            expected, found);
    exit(1);
}

void raise_error_with_token(char* reason, int token) {
    fprintf(stderr, "Error occured: '%s', on token ==> %c <==, exiting...", reason, token);
    free(reason);
    exit(1);
}

void jstkn_read_from_file_to_pool(JSON_node_pool* pool, FILE* file) {


    int token = fgetc(file);
    if (token == EOF) {
        raise_error("File is empty");
    }

    void* value;
    char* string = malloc(sizeof(char*));
    JSON_node* working_node = jsnd_create();

    // entering the file, if it fails, error occurs due to syntax error in the biggining of json
    // file

    if (token != token_left_curle_brace) {
        raise_error_with_token("Cannot read a file, file should begin with \' { \'", token);
        return;
    }

    // first-level loop, just to reach the end of the file.
    while ((token) != EOF) {

        //entered object
        if((token = fgetc(file)) == token_left_curle_brace){
            while ((token = fgetc(file)) != token_right_curle_brace) {

                //reading object
                JSON_node* child = jsnd_create();

                jsnd_append_child(working_node, child);
                

            }
        }

        // that is the second-level loop, the main one,
        // which will work until we reach the end of the file
        while ((token = fgetc(file)) != token_right_curle_brace) {
            // begin of the first object in the file
            if ((token = fgetc(file)) == token_left_curle_brace) {
                // checking if after proccessing the object, we have a comma or end of the of the
                // file
                if ((token = fgetc(file)) != token_comma) {
                    raise_error_unexpected_token(token_comma, token);
                    return;
                }
            }
        }
    }

    if (working_node == NULL) {
        raise_error_with_token("Root node was not found", token);
        return;
    }
}

char* construct_string_or_key(FILE* file){
    int token;
    
    string_builder_t* sb = sb_create();

    while((token = fgetc(file)) != token_double_quote){
        sb_append(sb, token);        
    }
    
    char* to_return = sb_copy_of_string_value(sb);
    sb_free(sb);
    return to_return;
}