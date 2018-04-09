#include "html_attribute.h"

char* get_attribute(myhtml_tree_node_t* node, const char* key)
{
  if(node == NULL) {
    return NULL;
  }
  myhtml_tree_attr_t* attr = myhtml_attribute_by_key(node, key, strlen(key));
  if(attr == NULL) {
    return NULL;
  }
  mycore_string_t* value = myhtml_attribute_value_string(attr);
  if(value == NULL) {
    return NULL;
  }

  char* data = (char*)html_malloc(value->length + 1);
  if(data == NULL) {
    return NULL;
  }
  strcpy(data, value->data);
  // User must free data.
  return data;
}

bool set_attribute(myhtml_tree_node_t* node, const char* key, const char* value)
{
  if(node == NULL) {
    return false;
  }

  myhtml_tree_attr_t* attr = myhtml_attribute_by_key(node, key, strlen(key));
  if(attr == NULL) {
#ifdef MODEST_HTML_DEBUG
    printf("set_attribute() - Insert new attribute %s=\"%s\"\n", key, value);
#endif
    myhtml_attribute_add(node, key, strlen(key), value, strlen(value), MyENCODING_UTF_8);
    return true;
  }
  else {
#ifdef MODEST_HTML_DEBUG
    printf("set_attribute() - Remove old attribute with key %s\n", key);
#endif
    // remove old attribute
    myhtml_attribute_remove(node, attr);
#ifdef MODEST_HTML_DEBUG
    printf("set_attribute() - Insert new attribute %s=\"%s\"\n", key, value);
#endif
    // insert new attribute
    myhtml_attribute_add(node, key, strlen(key), value, strlen(value), MyENCODING_UTF_8);
  }
  return true;
}

int html_get_attribute(html_workspace_t* workspace, int collection_index, const char* key)
{
  if(workspace == NULL) {
    fprintf(stderr, "html_get_attributes() - Empty workspace.\n");
    return -1;
  }

  myhtml_collection_t* collection = html_get_collection(workspace, collection_index);
  if(collection == NULL) {
    fprintf(stderr, "html_get_attributes() - Empty collection\n");
    return -1;
  }

  html_vec_str_t buffer;
  html_vec_init(&buffer);

  if(collection && collection->list && collection->length) {
    for(size_t i = 0; i < collection->length; i++) {
      myhtml_tree_node_t* node = collection->list[i];
      char* value = get_attribute(node, key);
      if(value == NULL) {
        continue;
      }
      size_t length = strlen(value);
      if(length > 0) {
        html_vec_push(&buffer, value);
      }
    }
  }

  html_vec_push(&workspace->buffers, buffer);

  return workspace->buffers.length - 1;
}

bool html_set_attribute(html_workspace_t* workspace, int collection_index, const char* key, const char* value)
{
  if(workspace == NULL) {
    fprintf(stderr, "html_set_attribute() - Empty workspace.\n");
    return false;
  }

  myhtml_collection_t* collection = html_get_collection(workspace, collection_index);
  if(collection == NULL) {
    fprintf(stderr, "html_set_attribute() - Empty collection\n");
    return false;
  }

  if(collection && collection->list && collection->length) {
    for(size_t i = 0; i < collection->length; i++) {
      myhtml_tree_node_t* node = collection->list[i];
      set_attribute(node, key, value);
    }
  }

  return true;
}