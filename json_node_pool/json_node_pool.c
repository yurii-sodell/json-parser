

#include "json_node_pool.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../json_node/json_node.h"
#include "../json_node/json_node_pool_shared.h"

#define BASIC_CAPACITY 128

typedef struct JSON_node_pool {
    JSON_node* root;
} JSON_node_pool;


void jsnd_pool_assign_node(JSON_node_pool* pool, JSON_node* node) {
    ensure_node_capacity(pool->root);
    jsnd_append_child(pool->root, node);
}

JSON_node_pool* jsnd_pool_create() {
    JSON_node_pool* pool = malloc(sizeof(JSON_node_pool));
    pool->root = jsnd_create_as_pool();
    return pool;
}

void jsnd_pool_print_nodes(JSON_node_pool* pool) {
    int len = get_node_child_count(pool->root);
    for (int i = 0; i < len; i++) {
      //  jsnd_node_print(&(pool->root) + i);
    }
}

int jsnd_pool_get_structure_size() { return sizeof(JSON_node_pool); };