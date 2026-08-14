#ifndef JSON_TOKENS_HANDLER_H
#define JSON_TOKENS_HANDLER_H

#include "../json_node/json_node.h"
#include "../json_node_pool/json_node_pool.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct json_tokens_handler json_tokens_handler;
void jstkn_read_from_file_to_pool(JSON_node_pool* pool, FILE* file);

#endif