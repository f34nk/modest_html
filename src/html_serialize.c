#include "html_serialize.h"

bool html_do_serialize_selector(myhtml_tree_node_t* node, html_vec_str_t* result)
{
  if(node == NULL) {
    return false;
  }

  const char* tag_name = myhtml_tag_name_by_id(node->tree, myhtml_node_tag_id(node), NULL);
  if(strcmp(tag_name, "-undef") == 0) {
    return true;
  }

  char* copy = (char*)html_malloc(strlen(tag_name) + 1 * sizeof(char*));
  if(copy) {
    strcpy(copy, tag_name);
    html_vec_push(result, copy);
  }

  myhtml_tree_node_t* parent_node = myhtml_node_parent(node);
  if(parent_node) {
    return html_do_serialize_selector(parent_node, result);
  }

  return false;
}

char* html_serialize_selector(myhtml_tree_node_t* node)
{
  if(node == NULL) {
    return NULL;
  }

  html_vec_str_t vec;
  html_vec_init(&vec);

  html_do_serialize_selector(node, &vec);

  html_vec_reverse(&vec);
  char* data = html_vec_join(&vec, " ");

  while(vec.length > 0) {
    char* buffer = html_vec_pop(&vec);
    html_free(buffer);
  }
  html_vec_deinit(&vec);

  return (char*)data;
}

mystatus_t html_serialize_dump_callback(const char* data, size_t data_length, void* file)
{
  fprintf((FILE*)file, "%.*s", (int)data_length, data);
  return MyCORE_STATUS_OK;
}

void html_serialize_dump(FILE* file, myhtml_tree_node_t* node)
{
  if(node == NULL) {
    fprintf(stderr, "html_serialize_dump() - Empty node\n");
    return;
  }

  myhtml_serialization_tree_callback(node, html_serialize_dump_callback, (void*)file);
  fprintf((FILE*)file, "\n");
}

#if 0
mystatus_t html_serialization_callback(const char* data, size_t data_length, void* result)
{
  int prev = strlen((char*)result);
  char* new_data = html_realloc((char*)result, prev + data_length + 1 * sizeof(char));
  // char *new_data = html_realloc((char*)result, prev + data_length * sizeof(char));
  if(new_data == NULL) {
    if(strlen((char*)result) == 0) {
      html_free(result);
      result = NULL;
    }
    return MyCORE_STATUS_ERROR;
  }

  strncat((char*)&new_data[prev], data, data_length);
  // result = new_data;

  return MyCORE_STATUS_OK;
}

char* html_serialize_node(myhtml_tree_node_t* node)
{
#if 0
  char* data = (char*)html_malloc(1 * sizeof(char));
  if(data == NULL) {
    return NULL;
  }
  *data = '\0';
#else
  char* data = (char*)html_calloc(1, 1);
#endif

  myhtml_serialization_tree_callback(node, html_serialization_callback, (void*)data);

  return data;
}
#else

mystatus_t html_serialization_callback(const char* data, size_t data_length, void* result)
{
  html_vec_str_t* vec = (html_vec_str_t*)result;

  char* copy = (char*)html_malloc(data_length + 1 * sizeof(char));
  if(copy) {
    strcpy(copy, data);
    html_vec_push(vec, copy);
    return MyCORE_STATUS_OK;
  }
  return MyCORE_STATUS_ERROR;
}

char* html_serialize_node(myhtml_tree_node_t* node)
{
  html_vec_str_t vec;
  html_vec_init(&vec);
  myhtml_serialization_tree_callback(node, html_serialization_callback, (void*)&vec);

  char* data = html_vec_join(&vec, "");

  while(vec.length > 0) {
    char* buffer = html_vec_pop(&vec);
    html_free(buffer);
  }
  html_vec_deinit(&vec);

  return data;
}

#endif

int html_serialize_collection(html_workspace_t* workspace, int collection_index)
{
  if(workspace == NULL) {
    fprintf(stderr, "html_serialize_collection() - Empty workspace.\n");
    return -1;
  }

  myhtml_collection_t* collection = (myhtml_collection_t*)html_get_collection(workspace, collection_index);
  if(collection == NULL) {
    fprintf(stderr, "html_serialize_collection() - Empty collection\n");
    return -1;
  }

  html_vec_str_t buffer;
  html_vec_init(&buffer);

  for(size_t i = 0; i < collection->length; i++) {

    myhtml_tree_node_t* node = collection->list[i];
    char* data = html_serialize_node(node);

    size_t length = strlen(data);
    if(length > 0) {
      html_vec_push(&buffer, data);
    }
    else {
      html_free(data);
    }
  }

  html_vec_push(&workspace->buffers, buffer);

  return workspace->buffers.length - 1;
}

int html_serialize_tree(html_workspace_t* workspace, int tree_index, const char* scope_name)
{
  html_vec_str_t buffer;
  html_vec_init(&buffer);

  myhtml_tree_t* tree = (myhtml_tree_t*)html_get_tree(workspace, tree_index);
  if(tree == NULL) {
    fprintf(stderr, "html_serialize_tree() - Empty tree\n");
    return -1;
  }

  myhtml_tree_node_t* scope_node = html_get_scope_node(workspace, tree_index, scope_name);
  if(scope_node == NULL) {
    fprintf(stderr, "html_serialize_tree() - No node for scope '%s'.\n", scope_name);
    return -1;
  }

  char* data = html_serialize_node(scope_node);
  size_t length = strlen(data);
  if(length > 0) {
    html_vec_push(&buffer, data);
  }
  // else {
  //   html_free(data);
  // }

  html_vec_push(&workspace->buffers, buffer);

  return workspace->buffers.length - 1;
}