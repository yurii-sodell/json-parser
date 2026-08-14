#include "json_node.h"
#include "json_node_pool_shared.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct JSON_node {
    char* key;

    enum node_state{
        NODE_EMPTY, NODE_BOOL, NODE_INT, NODE_STRING, NODE_OBJECT, NODE_ARRAY, POOL
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
    

    JSON_node** childNodes;
    JSON_node* parrentNode;
    int childs_counter;
    int childs_capacity;
    bool parantless;
} JSON_node;

#define BASIC_CAPACITY 1024

void ensure_node_capacity(JSON_node* node){
    int size = sizeof(JSON_node);
    int expected_capcaity = (node->childs_counter + 10) * size;
    while (node->childs_capacity < expected_capcaity) {
        int new_cap = node->childs_capacity / 2 + node->childs_capacity;
        void* tmp = realloc(node->childNodes, new_cap);
        node->childNodes = tmp;
        node->childs_capacity = new_cap;
    }
}

void jsnd_assign_key(JSON_node* node, char* key) {
    node->key = key;
}

JSON_node* jsnd_create() { 
    JSON_node* node = malloc(sizeof(JSON_node));
    node->key = NULL;
    node->node_state = NODE_EMPTY;
    node->childs_capacity = sizeof(JSON_node) * BASIC_CAPACITY;
    node->childNodes = malloc(node->childs_capacity);
    node->parrentNode = malloc(sizeof(JSON_node));
    node->parantless = false;
    return node;
}

JSON_node* jsnd_create_as_pool() { 
    JSON_node* node = malloc(sizeof(JSON_node));
    node->key = NULL;
    node->node_state = POOL;
    node->childs_capacity = sizeof(JSON_node) * BASIC_CAPACITY;
    node->childNodes = malloc(node->childs_capacity);
    node->parrentNode = NULL;
    node->parantless = true;
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
    ensure_node_capacity(node);
    int size_of_one_element = sizeof(JSON_node*); 
    memcpy(node->childNodes + node->childs_counter * size_of_one_element, child_node, size_of_one_element);
    node->childs_counter++;
    memcpy(child_node->parrentNode, node, sizeof(JSON_node*));
    
    }
}

bool jsnd_has_key(JSON_node* node){
    return node->node_state != NODE_EMPTY;
};

int jsnd_get_structure_size(){
    return sizeof(JSON_node);
};

void jsnd_node_print(JSON_node* node){
    
}

int get_node_child_count(JSON_node* node){
    return node->childs_counter;
}

int get_node_child_capacity(JSON_node* node){
    return node->childs_capacity;
}