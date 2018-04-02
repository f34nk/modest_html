#include "html_node.h"
#include "html_serialize.h"

void html_node_init(html_node_t *params)
{
  params->tag_name = NULL;
  params->selector = NULL;
  html_vec_init(&params->keys);
  html_map_init(&params->key_value);
  params->text = NULL;
  params->parent_selector = NULL;
}

void html_node_destroy(html_node_t *params)
{
  // free tag_name
  html_free(params->tag_name);
  // free selector
  html_free(params->selector);

  // free list of keys
  while(params->keys.length > 0) {
    char *buffer = html_vec_pop(&params->keys);
    html_free(buffer);
  }
  html_vec_deinit(&params->keys);

  // free key value map
  const char *key;
  map_iter_t iter = html_map_iter(&params->key_value);
  while ((key = html_map_next(&params->key_value, &iter))) {
    char *value = *html_map_get(&params->key_value, key);
    // html_free(key);
    html_free(value);
  }
  html_map_deinit(&params->key_value);

  // free text
  html_free(params->text);
  html_free(params->parent_selector);
}

int html_node_sort_callback(const void *a, const void *b) {
  return strcmp((char*)a, (char*)b);
}

bool html_node_get(myhtml_tree_node_t* node, html_node_t *params)
{
  if(node == NULL) {
    return false;
  }

  const char *tag_name = myhtml_tag_name_by_id(node->tree, myhtml_node_tag_id(node), NULL);
  if(strcmp(tag_name, "-text") == 0){
    // this is a text node
    // params->tag_name = NULL;
    char *tag_name_copy;
    html_string_copy(tag_name, tag_name_copy);
    params->tag_name = tag_name_copy;

    mycore_string_t *string = myhtml_node_string(node);
    const char *data = mycore_string_data(string);
    char *data_copy;
    html_string_copy(data, data_copy)
    params->text = data_copy;

    myhtml_tree_node_t *parent_node = myhtml_node_parent(node);
    params->parent_selector  = html_serialize_selector(parent_node);

    return true;
  }
  else {
    // this is NOT a text node
    params->text = NULL;
    char *tag_name_copy;
    html_string_copy(tag_name, tag_name_copy);
    params->tag_name = tag_name_copy;

    params->selector = html_serialize_selector(node);

    myhtml_tree_attr_t *attr = myhtml_node_attribute_first(node);
    
    while (attr) {
      const char *key = myhtml_attribute_key(attr, NULL);
      if(key) {
        char *key_copy;
        html_string_copy(key, key_copy);
        html_vec_push(&params->keys, key_copy);

        const char *value = myhtml_attribute_value(attr, NULL);
        if(value){
          // char *key_copy;
          // html_string_copy(key, key_copy);
          char *value_copy;
          html_string_copy(value, value_copy);
          html_map_set(&params->key_value, key, value_copy);
        }
      }
      attr = myhtml_attribute_next(attr);
    } // while attr

    // sort keys
    html_vec_sort(&params->keys, html_node_sort_callback);
    return true;
  }

  return false;
}

bool html_node_is_text(html_node_t *params)
{
  if(params == NULL) {
    return false;
  }
  return (strcmp(params->tag_name, "-text") == 0) ? true : false;
}

bool html_node_has_attributes(html_node_t *params)
{
  if(params == NULL) {
    return false;
  }
  return (params->keys.length > 0) ? true : false;
}

void html_node_dump(FILE *file, html_node_t *params)
{
  fprintf((FILE*)file, "html_node_dump()\n");
  if(params->tag_name) {
    fprintf((FILE*)file, "\ttag_name = %s\n", params->tag_name);
    fprintf((FILE*)file, "\tselector = %s\n", params->selector);
  }

  if(params->keys.length > 0) {

    fprintf((FILE*)file, "\tkeys: ");
    int i;
    char *val;
    html_vec_foreach(&params->keys, val, i) {
      char *key = params->keys.data[i];
      fprintf((FILE*)file, "%s", key);
      if(i < params->keys.length - 1) {
        fprintf((FILE*)file, ", ");
      }
    }
    fprintf((FILE*)file, "\n");

    i = 0;
    val = NULL;
    fprintf((FILE*)file, "\tkey_value:\n");
    html_vec_foreach(&params->keys, val, i) {
      char *key = params->keys.data[i];
      char *value = (char*)*html_map_get(&params->key_value, key);
      fprintf((FILE*)file, "\t%d: %s = %s\n", i, key, value);
    }
  }

  if(params->text) {
    fprintf((FILE*)file, "\ttext = %s\n", params->text);
  }
}