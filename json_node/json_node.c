#include "json_node.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct JSON_node {
    char* key;

    enum node_state{
        NODE_EMPTY, NODE_BOOL, NODE_INT, NODE_STRING, NODE_OBJECT, NODE_ARRAY
    } node_state;

    union value {
        char* string;
        bool boolean;
        int integer;
        float floating;
    } value;

    union array_value
    {
        char** string;
        bool* boolean;
        int* integer;
        float* floating;
    };
    

    JSON_node** childNode;
    JSON_node* parrentNode;
    int childs_counter;

} JSON_node;

void jsnd_assign_key(JSON_node* node, char* key) {
    node->key = key;
}

JSON_node* jsnd_create() { 
    JSON_node* node = malloc(sizeof(JSON_node*));
    node->key = NULL;
    node->node_state = NODE_EMPTY;
    node->childNode = malloc(sizeof(JSON_node**));
    node->parrentNode = malloc(sizeof(JSON_node*));
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

void jsnd_append_child(JSON_node* node, JSON_node* child_node) {
    if(node->node_state == NODE_EMPTY){
    node->childs_counter++;
    memmove(node->childNode + sizeof(JSON_node*) * node->childs_counter, child_node, sizeof(JSON_node*));
    child_node->parrentNode = node;
    }
}

bool jsnd_has_key(JSON_node* node){
    return node->node_state != NODE_EMPTY;
};