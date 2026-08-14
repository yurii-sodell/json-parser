

#include "json_node_pool.h"
#include "../json_node/json_node.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct JSON_node_pool {
    JSON_node** roots;
    int length;
} JSON_node_pool;

void jsnd_pool_assign_node(JSON_node_pool* pool, JSON_node* node) {}


JSON_node_pool* jsnd_pool_create() {
    JSON_node_pool* pool = malloc(sizeof(JSON_node_pool*));
    pool->roots = malloc(sizeof(JSON_node**));
    return pool;
}
