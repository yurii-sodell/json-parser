#include "json_node.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct jsnull {
    unsigned char dummy;
} jsnull;

typedef enum node_state {
        NODE_EMPTY, //0
        NODE_BOOL, //1
        NODE_NUMBER, //2
        NODE_STRING, //3
        NODE_VALUE_NULL, //4
        NODE_OBJECT_AS_VALUE, //5
        NODE_ARRAY_AS_VALUE, //6
    } node_state;

typedef struct JSON_node {
    char* key;

    node_state node_state;

    union value {
        char* string;
        bool boolean;
        long double number;
        jsnull* null;
    } value;

    union array_value {
        char** string;
        bool* boolean;
        long double* number;
    };

    JSON_node** childNodes;
    JSON_node* parrentNode;
    int childs_counter;
    int childs_capacity;
    bool is_root;
} JSON_node;

#define JSON_NODE_VALUE_BASIC_CAPACITY 20

void ensure_node_capacity(JSON_node* node) {
    int size = sizeof(JSON_node);
    int expected_capcaity = (node->childs_counter + 10) * size;
    while (node->childs_capacity < expected_capcaity) {
        int new_cap = node->childs_capacity / 2 + node->childs_capacity;
        void* tmp = realloc(node->childNodes, new_cap);
        node->childNodes = tmp;
        node->childs_capacity = new_cap;
    }
}

void jsnd_assign_key(JSON_node* node, char* key) { node->key = key; }

JSON_node* jsnd_create() {
    JSON_node* node = malloc(sizeof(JSON_node));
    node->key = NULL;
    node->node_state = NODE_EMPTY;
    node->childs_capacity = sizeof(JSON_node*) * JSON_NODE_VALUE_BASIC_CAPACITY;
    node->childNodes = malloc(node->childs_capacity);
    node->parrentNode = malloc(sizeof(JSON_node));
    node->is_root = false;
    node->childs_counter = 0;
    return node;
}

JSON_node* jsnd_get_child(JSON_node* node, int i) {
    if (node == NULL) return NULL;
    if( i > node->childs_counter) return NULL;
    if (i < 0) return NULL;
    return node->childNodes[i];
}

void jsnd_assign_number(JSON_node* node, long double value) {
    if (node->node_state == NODE_EMPTY) {
        node->value.number = value;
        node->node_state = NODE_NUMBER;
    }
}
void jsnd_assign_string(JSON_node* node, char* value) {
    if (node->node_state == NODE_EMPTY) {
        node->value.string = value;
        node->node_state = NODE_STRING;
    }
}
void jsnd_assign_bool(JSON_node* node, bool value) {
    if (node->node_state == NODE_EMPTY) {
        node->value.boolean = value;
        node->node_state = NODE_BOOL;
    }
}

void jsnd_assign_value_null(JSON_node* node) {
    jsnull* value = malloc(sizeof(jsnull));
    if (node->node_state == NODE_EMPTY) {
        node->value.null = value;
        node->node_state = NODE_VALUE_NULL;
    }
}

void jsnd_append_child(JSON_node* node, JSON_node* child_node) {
    if (node->node_state == NODE_EMPTY || node->node_state == NODE_OBJECT_AS_VALUE) {
        ensure_node_capacity(node);
        node->node_state = NODE_OBJECT_AS_VALUE;
        int size_of_one_element = sizeof(JSON_node*);
        node->childNodes[node->childs_counter] = child_node;
        node->childs_counter++;
        memcpy(child_node->parrentNode, node, sizeof(JSON_node*));
    }
}

void jsnd_mark_as_root(JSON_node* node){
    node->is_root = true;
}

bool jsnd_has_key(JSON_node* node) { return node->node_state != NODE_EMPTY; };

int jsnd_get_structure_size() { return sizeof(JSON_node); };

void jsnd_print_nodes_reqursively(JSON_node* node) {
    
    if(node->is_root){

        int lenc = node->childs_counter;
        for (int i = 0; i < lenc; i++) {
            JSON_node* child_node = jsnd_get_child(node, i);
            jsnd_print_nodes_reqursively(child_node);
        }
    } else {
        switch (node->node_state) {
            case NODE_EMPTY:
                printf("\nNode has no value");
                break;
            case NODE_BOOL:
                printf("\nKey: %20s | Value: %20s", node->key,
                       node->value.boolean ? "true" : "false");
                break;
            case NODE_NUMBER:
                printf("\nKey: %20s | Value: %.10f", node->key, (double) node->value.number);
                break;
            case NODE_STRING:
                printf("\nKey: %20s | Value: %20s", node->key, node->value.string);
                break;
            case NODE_OBJECT_AS_VALUE:
                printf("\n%2c->Key: %20s", ' ', node->key);
                int len_obj = node->childs_counter;
                printf("\nLen: %d", node->childs_counter);
                for (int i = 0; i < len_obj; i++) {
                    jsnd_print_nodes_reqursively(node->childNodes[i]);
                }
                break;
            case NODE_VALUE_NULL:
                printf("\nKey: %20s | Value: null", node->key);
                break;
            case NODE_ARRAY_AS_VALUE:
                printf("\n->Key: %20s", node->key);
                int len_arr = node->childs_counter;
                for (int i = 0; i < len_arr; i++) {
                    jsnd_print_nodes_reqursively(node->childNodes[i]);
                }
                break;
            default:
                fprintf(stderr, "\nUnknown type of node : %d", node->node_state);
                break;
        }
    }
}

int jsnd_get_node_child_count(JSON_node* node) { return node->childs_counter; }
char* jsnd_get_key(JSON_node* node){
    return node->key;
};

int get_node_child_capacity(JSON_node* node) { return node->childs_capacity; }

int jsnd_get_type(JSON_node* node) { return node->node_state; }