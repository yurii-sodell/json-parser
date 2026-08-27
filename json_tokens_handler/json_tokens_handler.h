#ifndef JSON_TOKENS_HANDLER_H
#define JSON_TOKENS_HANDLER_H

#include "../json_node/json_node.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct json_tokens_handler json_tokens_handler;
JSON_node* jstkn_read_from_file(FILE* file);

#endif