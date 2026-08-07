
typedef struct JSON_node JSON_node;

JSON_node* jsnd_create();
void jsnd_assign_key(JSON_node* node, char* key);
void jsnd_assign_int(JSON_node* node, int value);
void jsnd_assign_string(JSON_node* node, char* value);
void jsnd_assign_bool(JSON_node* node, bool value);
void jsnd_assign_child(JSON_node* node, JSON_node* child_node);
bool jsnd_has_key(JSON_node* node);