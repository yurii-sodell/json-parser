
#ifndef JSPARSER_JSON_NODE_POOL_SHARED_H
#define JSPARSER_JSON_NODE_POOL_SHARED_H

typedef struct JSON_node JSON_node;

JSON_node* jsnd_create();
JSON_node* jsnd_create_as_pool();

void ensure_node_capacity(JSON_node* node);
int get_node_child_count(JSON_node* node);
int get_node_child_capacity(JSON_node* node);

#endif //JSPARSER_JSON_NODE_POOL_SHARED_H