#include "json_node.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include <stdbool.h>

typedef struct JSON_node {
    char* key;

    enum node_state{
        NODE_EMPTY, NODE_BOOL, NODE_INT, NODE_STRING, NODE_CHILD
    } node_state;

    union value {
        char* string;
        bool boolean;
        int integer;
        JSON_node* childNode;
    } value;

} JSON_node;

void jsnd_assign_key(JSON_node* node, char* key) {
    node->key = key;
}

JSON_node* jsnd_create() { 
    JSON_node* node = malloc(sizeof(JSON_node*));
    node->key = NULL;
    node->node_state = NODE_EMPTY;
    return node;
}

void jsnd_assign_int(JSON_node* node, int value){
    if(node->node_state == NODE_EMPTY){
    node->value.integer = value; 
    node->node_state = NODE_INT;
    }
}
void jsnd_assign_string(JSON_node* node, char* value) {
    if(node->node_state == NODE_EMPTY){
    node->value.string = value;
    node->node_state = NODE_STRING;
    }
}
void jsnd_assign_bool(JSON_node* node, bool value) {
     if(node->node_state == NODE_EMPTY){
     node->value.boolean = value;
    node->node_state = NODE_BOOL;
    }
}

void jsnd_assign_child(JSON_node* node, JSON_node* child_node) {
    if(node->node_state == NODE_EMPTY){
     node->value.childNode = child_node;
    node->node_state = NODE_CHILD;
    }
}

bool jsnd_has_key(JSON_node* node){
    return node->node_state != NODE_EMPTY;
};