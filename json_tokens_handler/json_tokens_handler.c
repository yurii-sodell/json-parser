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

// ignored outside the text
#define TOKEN_END_OF_LINE_DEFAULT '\n'
#define TOKEN_TAB '\t'
#define TOKEN_SPACE ' '

typedef struct json_tokens_handler {
} json_tokens_handler;

#define LENGTH_OF_TRUE 4
#define LENGTH_OF_FALSE 5
char true_chars[LENGTH_OF_TRUE] = {'t', 'r', 'u', 'e'};
char false_chars[LENGTH_OF_FALSE] = {'f', 'a', 'l', 's', 'e'};

void decide_on_value_and_assign(FILE* file, JSON_node* branch);
JSON_node* build_as_object(FILE* file, JSON_node* branch);
JSON_node* build_as_array(FILE* file, JSON_node* branch);

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

bool is_ignored(char token) {
    return (token == TOKEN_SPACE || token == TOKEN_END_OF_LINE_DEFAULT || token == TOKEN_TAB ||
            token == EOF);
}

void read_and_skip_ignored_tokens(FILE* file) {
    int token;
    while (is_ignored((token = fgetc(file)))) {
        // doing nothing, just skipping garbage
    }
    ungetc(token, file);
}

void validate_file_or_exit(FILE* file) {
    int token = fgetc(file);

    if (token == EOF) {
        raise_error("File is empty");
    }
    read_and_skip_ignored_tokens(file);
    // entering the file, if it fails, error occurs due to syntax error in the biggining of json
    // file

    if (token != TOKEN_LEFT_CURLY_BRACE && token != TOKEN_LEFT_SQUARE_BRACE) {
        raise_error_with_token("Cannot read a file, file should begin with \'{\' or \'[\'", token);
        return;
    }
    ungetc(token, file);
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

int construct_int_or_float(FILE* file) {}

char* construct_boolean(FILE* file) {}

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

int has_reached_end_of_file(char token) { return token == EOF; }

JSON_node* build_as_array(FILE* file, JSON_node* branch) { read_and_skip_ignored_tokens(file); }

bool try_to_build_boolean(FILE* file, char t_or_f_char) {
    int counter = 1;

    switch (t_or_f_char) {
        case 't':
            raise_error_with_token("Failed on building value \'true\'", t_or_f_char);
            while (counter < LENGTH_OF_TRUE) {
                char target = (char)fgetc(file);
                if (true_chars[counter] != target) {
                    raise_error_unexpected_token(true_chars[counter], target);
                    counter++;
                }
            }
            return true;
        case 'f':
            raise_error_with_token("Failed on building value \'false\'", t_or_f_char);
            while (counter < LENGTH_OF_FALSE) {
                char target = (char)fgetc(file);
                if (false_chars[counter] != target) {
                    raise_error_unexpected_token(true_chars[counter], target);
                    counter++;
                }
            }
            return false;
        default:
            raise_error_with_token("Expected t or f", t_or_f_char);
            break;
    }
}


void decide_on_value_and_assign(FILE* file, JSON_node* branch) {
    read_and_skip_ignored_tokens(file);
    int token = fgetc(file);
    if (token == 't' || token == 'f') {
        bool built_bool = try_to_build_boolean(file, token);
        jsnd_assign_bool(branch, built_bool);
        return;
    }

    if(token == TOKEN_DOUBLE_QUOTE){
        ungetc(token, file);
        char* string_value = construct_string_or_key(file);
        jsnd_assign_string(branch, string_value);
        return;
    }

    if(token == TOKEN_LEFT_CURLY_BRACE){
        JSON_node* child = jsnd_create();
        build_as_object(file, child);
        jsnd_append_child(branch, child);
        return;
    }

    if(token == TOKEN_LEFT_SQUARE_BRACE){
        JSON_node* child = jsnd_create();
        build_as_array(file, child);
        jsnd_append_child(branch, child);
        return;
    }
}

JSON_node* build_as_object(FILE* file, JSON_node* branch) {
    read_and_skip_ignored_tokens(file);
    int token = fgetc(file);
    if (token == TOKEN_DOUBLE_QUOTE) {  // == "
        // building the key;
        ungetc(token, file);
        char* key = construct_string_or_key(file);
        jsnd_assign_key(branch, key);
        token = fgetc(file);
        // now we are going to build value
        if (token == TOKEN_COLON) {
            decide_on_value_and_assign(file, branch);
        } else {
            raise_error_unexpected_token(':', token);
        }

    } else {
        raise_error("Error occured while building a key");
    }
}

bool jstkn_read_from_file_to_branch(FILE* file, JSON_node* branch) {
    read_and_skip_ignored_tokens(file);
    bool has_reached_end = false;
    if (branch == NULL) {
        branch = jsnd_create();
    }

    int token = fgetc(file);
    char x = (char)token;
    switch (x) {
        case TOKEN_LEFT_CURLY_BRACE:
            build_as_object(file, branch);
            break;
        case TOKEN_RIGHT_CURLY_BRACE:
            build_as_array(file, branch);
            break;
        default:
            raise_error_with_token("Unexpected token", x);
            break;
    }
    return has_reached_end_of_file(token);
}

void jstkn_read_from_file_to_pool(JSON_node_pool* pool, FILE* file) {
    validate_file_or_exit(file);
    // the file biggining looks fine, now we are going to read tokens from the beggining
    bool reading = true;
    while (reading) {
        read_and_skip_ignored_tokens(file);
        JSON_node* new_branch = NULL;
        reading = jstkn_read_from_file_to_branch(file, new_branch);
        if (new_branch != NULL) {
            jsnd_pool_assign_node(pool, new_branch);
        }
    }
}