#include "json_tokens_handler.h"

#include <stdlib.h>

#include "../c-string-builder/string_builder_t.h"
#include "../json_node_pool/json_node_pool.h"

#define TOKEN_COLON ':'

// this group is valid after ':'
#define TOKEN_DOUBLE_QUOTE '\"'
#define TOKEN_LEFT_SQUARE_BRACE '['
#define TOKEN_TRUE "true"
#define TOKEN_FALSE "false"

// this must be seen only under certain circumstances such as beginning of object, end of it or
// inside text
#define TOKEN_SINGLE_QUOTE '\''
#define TOKEN_LEFT_CURLY_BRACE '{'
#define TOKEN_RIGHT_CURLY_BRACE '}'
#define TOKEN_RIGHT_SQUARE_BRACE ']'
#define TOKEN_COMMA ','

//ignored outside the text
#define TOKEN_END_OF_LINE_DEFAULT '\n'
#define TOKEN_TAB '\t'
#define TOKEN_SPACE ' '


typedef struct json_tokens_handler {
} json_tokens_handler;

void raise_error(char* reason) {
    fprintf(stderr, "Error occured: => %s <=, exiting...", reason);
    free(reason);
    exit(1);
}

void raise_error_unexpected_end_of_the_file() {
    fprintf(stderr, "Parser reached the end of the file before finishing parce");
    exit(1);
}

void raise_error_unexpected_token(char expected, char found) {
    fprintf(stderr, "Unexpected token, expected => %c <=, on token => %c <=, exiting...", expected,
            found);
    exit(1);
}

void raise_error_with_token(char* reason, int token) {
    fprintf(stderr, "Error occured: => %s <=, on token => %c <=, exiting...", reason, token);
    free(reason);
    exit(1);
}

void check_unexpected_end(char token) {
    if (token == EOF) {
        raise_error_unexpected_end_of_the_file();
    }
}

bool is_ignored(char token){
    return (token == TOKEN_SPACE || token == TOKEN_END_OF_LINE_DEFAULT || token == TOKEN_TAB);
}

char get_closing_token(int token) {
    char x = (char)token;
    switch (x) {
        case TOKEN_LEFT_CURLY_BRACE:
            return TOKEN_RIGHT_CURLY_BRACE;
            break;
        case TOKEN_LEFT_SQUARE_BRACE:
            return TOKEN_RIGHT_SQUARE_BRACE;
        case TOKEN_DOUBLE_QUOTE:
            return TOKEN_DOUBLE_QUOTE;
        default:
            return ' ';
            break;
    }
}

int construct_int_or_float(FILE* file){

}

char* construct_boolean(FILE* file){
    
}

char* construct_string_or_key(FILE* file) {
    int token;
    string_builder_t* sb = sb_create();
    sb_append_char(sb, fgetc(file));
    while ((token = fgetc(file)) != TOKEN_DOUBLE_QUOTE) {
        sb_append_char(sb, token);
    }
    sb_append_char(sb, token);
    char* to_return = sb_copy_of_string_value(sb);
    sb_free(sb);
    return to_return;
}

void assign_value_based_on_tokens(FILE* file, JSON_node* node) {}

void jstkn_read_from_file_to_pool(JSON_node_pool* pool, FILE* file) {
    int token = fgetc(file);
    
    if (token == EOF) {
        raise_error("File is empty");
    }
    // entering the file, if it fails, error occurs due to syntax error in the biggining of json
    // file

    if (token != TOKEN_LEFT_CURLY_BRACE && token != TOKEN_LEFT_SQUARE_BRACE) {
        raise_error_with_token("Cannot read a file, file should begin with \'{\' or \'[\'", token);
        return;
    }
    

    JSON_node* working_node = jsnd_create();
    

    JSON_node* working_child_node = NULL;
    char expected_root_token_closing_brace = get_closing_token(token);
    char expected_object_token_closing_brace;
    // the file biggining looks fine, now we are going to read tokens
    //  first-level loop, just to reach the end of the file. BUT NOT THE END OF THE TREE
    while ((token) != EOF) {
        
        // entering tree
        //  that is the second-level loop, the main one,
        //  which will work until we reach the END OF THE TREE

        while ((token = fgetc(file)) != expected_root_token_closing_brace) {
            if (is_ignored(token)) continue;
            check_unexpected_end(token);
            
            // begin of the first object in the file
            if (token == TOKEN_DOUBLE_QUOTE && working_child_node == NULL) {  // == "
                ungetc(token, file);
                working_child_node = jsnd_create();
                char* key = construct_string_or_key(file);
                jsnd_assign_key(working_child_node, key);
                token = fgetc(file);
                printf("\nPrinting key: => %s <=", key);
                // now we are going to build value
                if (token == TOKEN_COLON) {
                    
                } else {
                    raise_error_unexpected_token(TOKEN_COLON, token);
                }
                
                jsnd_append_child(working_node, working_child_node);
            }
        }
    }
}