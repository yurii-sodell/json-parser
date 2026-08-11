#include "../json_node/json_node.h"
typedef struct JSON_node_pool JSON_node_pool;
#include <stdio.h>

JSON_node_pool* jsnd_pool_create();
void jsnd_pool_read_from_file(JSON_node_pool* pool, FILE* file);
