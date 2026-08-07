#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "json_node/json_node.h"

void raise_error(char* reason, int code) {
    printf("%s", reason);
    free(reason);
    exit(code);
}

void raise_error_with_token(char* reason, int token, int code) {
    printf("Error occured: '%s', on token ==> %c <==", reason, token);
    free(reason);
    exit(code);
}

int main(int argcounter, char* argvalue[]) {
    if (argcounter < 2) {
        printf("Filename is not provided.");
        exit(-1);
    }

    char* filename = argvalue[1];
    FILE* file = fopen(filename, "r");

    if (file == NULL) {
        printf("Cannot open the file %s", filename);
        exit(2);
    }

    char token_double_quote = '\"';
    char token_single_quote = '\'';
    char token_left_curle_brace = '{';
    char token_right_curle_brace = '}';
    char token_left_square_brace = '[';
    char token_right_square_brace = ']';
    char* token_true = "true";
    char* token_false = "false";
    char token_colon = ':';
    char token_end_of_line = '\\n';

    int token;
    JSON_node* working_node = NULL;

    while ((token = fgetc(file)) != EOF) {
        putchar(token);
        if (token == token_left_curle_brace) {
            printf("We got here: pringing charss \n");

            working_node = jsnd_create();
            
            while ((token = fgetc(file)) != token_right_curle_brace) {
                putchar(token);
            }
        }

        if (working_node == NULL) {
            raise_error_with_token("Root node was not found", token, 4);
        }
    }
}
