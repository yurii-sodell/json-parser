#ifndef JSPARSER_JSON_POOL_H
#define JSPARSER_JSON_POOL_H

#include "../json_node/json_node.h"

typedef struct JSON_node_pool JSON_node_pool;
JSON_node_pool* jsnd_pool_create();

#endif //JSPARSER_JSON_POOL_H