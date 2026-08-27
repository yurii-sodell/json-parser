#ifndef JSPARSER_JSON_NODE_H
#define JSPARSER_JSON_NODE_H

#include <stdbool.h>

typedef struct JSON_node JSON_node;

JSON_node* jsnd_create();

void jsnd_assign_key(JSON_node* node, char* key);
void jsnd_assign_number(JSON_node* node, long double value);
void jsnd_assign_string(JSON_node* node, char* value);
void jsnd_assign_bool(JSON_node* node, bool value);
void jsnd_assign_value_null(JSON_node* node);
void jsnd_append_child(JSON_node* node, JSON_node* child_node);
bool jsnd_has_key(JSON_node* node);
int jsnd_get_structure_size();
int jsnd_get_type(JSON_node* node);
int jsnd_get_node_child_count(JSON_node* node);
JSON_node* jsnd_get_child(JSON_node* node, int i);
char* jsnd_get_key(JSON_node* node);
void jsnd_mark_as_root(JSON_node* node);

void jsnd_print_nodes_reqursively(JSON_node* node);

#endif // JSPARSER_JSON_NODE_H