#ifndef JSPARSER_JSON_POOL_H
#define JSPARSER_JSON_POOL_H

#include "../json_node/json_node.h"

typedef struct JSON_node_pool JSON_node_pool;
JSON_node_pool* jsnd_pool_create();
int jsnd_pool_get_structure_size();
void jsnd_pool_assign_node(JSON_node_pool* pool, JSON_node* node);

#endif //JSPARSER_JSON_POOL_H