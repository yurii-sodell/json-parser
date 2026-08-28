#include "json_tokens_handler.h"

#include <ctype.h>
#include <stdlib.h>

#include "../c-string-builder/string_builder_t.h"

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
#define TOKEN_DOT '.'

// ignored outside the text
#define TOKEN_END_OF_LINE_DEFAULT '\n'
#define TOKEN_TAB '\t'
#define TOKEN_SPACE ' '

typedef struct json_tokens_handler {
} json_tokens_handler;

#define LENGTH_OF_TRUE 4
#define LENGTH_OF_FALSE 5
#define LENGTH_OF_NULL 4

char true_chars[LENGTH_OF_TRUE] = {'t', 'r', 'u', 'e'};
char false_chars[LENGTH_OF_FALSE] = {'f', 'a', 'l', 's', 'e'};
char null_chars[LENGTH_OF_NULL] = {'n', 'u', 'l', 'l'};

void decide_on_value_and_assign(FILE* file, JSON_node* branch);
JSON_node* build_as_array(FILE* file, JSON_node* branch);
JSON_node* build_as_object(FILE* file, JSON_node* root);

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

void check_unexpected_end(FILE* file) {
    int token = fgetc(file);
    if (token == EOF) raise_error_unexpected_end_of_the_file();
    ungetc(token, file);
}

bool is_ignored(char token) {
    return (token == TOKEN_SPACE || token == TOKEN_END_OF_LINE_DEFAULT || token == TOKEN_TAB);
}

void read_and_skip_ignored_tokens(FILE* file) {
    int token;
    while (is_ignored((token = fgetc(file)))) {
        // doing nothing, just skipping garbage
    }
    ungetc(token, file);
}

int fgetc_checked_cleared(FILE* file) {
    read_and_skip_ignored_tokens(file);
    check_unexpected_end(file);
    return fgetc(file);
}

int fgetc_cleared(FILE* file) {
    read_and_skip_ignored_tokens(file);
    return fgetc(file);
}

int fgetc_checked(FILE* file) {
    check_unexpected_end(file);
    return fgetc(file);
}

void validate_file_or_exit(FILE* file) {
    int token = fgetc_cleared(file);
    if (token == EOF) {
        raise_error("File is empty");
    }
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

char* construct_string_or_key(FILE* file) {
    int token;
    string_builder_t* sb = sb_create();
    sb_append_char(sb, fgetc_checked(file));
    while ((token = fgetc_checked(file)) != TOKEN_DOUBLE_QUOTE) {
        sb_append_char(sb, token);
    }
    sb_append_char(sb, token);
    char* to_return = sb_copy_of_string_value(sb);
    sb_free(sb);
    return to_return;
}

int has_reached_end_of_file(char token) { return token == EOF; }

bool try_to_build_boolean(FILE* file, char t_or_f_char) {
    int counter = 0;
    switch (t_or_f_char) {
        case 't':
            while (counter < LENGTH_OF_TRUE) {
                char target = (char)fgetc_checked(file);
                if (true_chars[counter] != target) {
                    raise_error_with_token("Failed on building value \'true\'", t_or_f_char);
                }
                counter++;
            }
            return true;
        case 'f':
            while (counter < LENGTH_OF_FALSE) {
                char target = (char)fgetc_checked(file);
                if (false_chars[counter] != target) {
                    raise_error_with_token("Failed on building value \'false\'", t_or_f_char);
                }
                counter++;
            }
            return false;
        default:
            
            raise_error_with_token("Expected t or f", t_or_f_char);
            break;
    }
}



long double construct_number(FILE* file) {
    int token;
    string_builder_t* value = sb_create();
    bool is_dot_set = 0;
    while ((token = fgetc_checked_cleared(file)) != TOKEN_COMMA &&
           token != TOKEN_RIGHT_CURLY_BRACE && token != TOKEN_RIGHT_SQUARE_BRACE) {
        if (isdigit(token)) {
            sb_append_char(value, token);
            continue;
        }
        if (token == TOKEN_DOT) {
            if (!is_dot_set) {
                sb_append_char(value, token);
                is_dot_set = 1;
                continue;
            }
            raise_error("Found second dot => \'.\' <=, while parsin digit");
        }
        raise_error_with_token("Unexpected token while parcing digit", token);
    }
    ungetc(token, file);
    char* read_string = sb_copy_of_string_value(value);
    long double number = strtold(read_string, NULL);
    if (read_string != NULL) {
        free(read_string);
    }
    sb_free(value);
    return number;
}

bool is_null_built(FILE* file) {
    for (int i = 0; i < LENGTH_OF_NULL; i++) {
        char target = (char)fgetc_checked(file);
        if (null_chars[i] != target) {
            raise_error_unexpected_token(null_chars[i], target);
        }
    }
    return true;
}

void decide_on_value_and_assign(FILE* file, JSON_node* branch) {
    int token = fgetc_checked_cleared(file);

    if (token == 't' || token == 'f') {
        ungetc(token, file);
        bool built_bool = try_to_build_boolean(file, token);
        jsnd_assign_bool(branch, built_bool);
        return;
    }

    if (token == 'n') {
        ungetc(token, file);
        if (is_null_built(file)) {
            jsnd_assign_value_null(branch);
        }
        return;
    }

    if (isdigit(token)) {
        ungetc(token, file);
        long double x = construct_number(file);
        jsnd_assign_number(branch, x);
        return;
    }

    if (token == TOKEN_DOUBLE_QUOTE) {
        ungetc(token, file);
        char* string_value = construct_string_or_key(file);
        jsnd_assign_string(branch, string_value);
        return;
    }

    if (token == TOKEN_LEFT_CURLY_BRACE) {
        build_as_object(file, branch);
        return;
    }

    if (token == TOKEN_LEFT_SQUARE_BRACE) {
        build_as_array(file, branch);
        return;
    }
}

JSON_node* build_as_object(FILE* file, JSON_node* root) {
    read_and_skip_ignored_tokens(file);

    JSON_node* child = jsnd_create();
    int token = fgetc(file);
    if (token == TOKEN_DOUBLE_QUOTE) {  // == "
        // building the key;
        ungetc(token, file);
        char* key = construct_string_or_key(file);
        jsnd_assign_key(child, key);
        token = fgetc_checked_cleared(file);
        // now we are going to build value
        if (token == TOKEN_COLON) {
            decide_on_value_and_assign(file, child);
            jsnd_append_child(root, child);
        } else {
            raise_error_unexpected_token(TOKEN_COLON, token);  // ':'
        }
    } else {
        raise_error("Error occured while building a key");
    }

    token = fgetc_checked_cleared(file);
    if (token == TOKEN_RIGHT_CURLY_BRACE) return root;
    if (token != TOKEN_COMMA) raise_error_unexpected_token(TOKEN_COMMA, token);
    build_as_object(file, root);
}

JSON_node* build_as_array(FILE* file, JSON_node* branch) {
    read_and_skip_ignored_tokens(file);
}

JSON_node* jstkn_read_from_file_by_token(FILE* file) {
    JSON_node* root;
    bool has_reached_end = false;
    int token = fgetc(file);
    char x = (char)token;
    switch (x) {
        case TOKEN_LEFT_CURLY_BRACE:
            root = jsnd_create();
            build_as_object(file, root);
            break;
        case TOKEN_LEFT_SQUARE_BRACE:
            root = jsnd_create();
            build_as_array(file, root);
            break;
        default:
            raise_error_with_token("Unexpected token", x);
            break;
    }
    return root;
}

JSON_node* jstkn_read_from_file(FILE* file) {
    validate_file_or_exit(file);
    // the file biggining looks fine, now we are going to read tokens from the beggining
    JSON_node* root = jstkn_read_from_file_by_token(file);
    jsnd_mark_as_root(root);
    return root;
}
