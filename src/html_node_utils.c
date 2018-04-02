#include "html_node_utils.h"

bool html_node_is_text(myhtml_tree_node_t* node)
{
  if(node){
    const char *tag_name = myhtml_tag_name_by_id(node->tree, myhtml_node_tag_id(node), NULL);
    if(strcmp(tag_name, "-text") == 0){
      return true;
    }
  }
  return false;
}

bool html_node_has_attributes(myhtml_tree_node_t* node)
{
  if(node){
    myhtml_tree_attr_t *attr = myhtml_node_attribute_first(node);
    return (attr) ? true : false;
  }
  return false;
}

void* html_node_get_attributes(html_workspace_t *workspace, myhtml_tree_node_t* node)
{
  if(node == NULL) {
  	return NULL;
  }
  
  html_vec_int_t indices;
  html_vec_init(&indices);

  myhtml_tree_attr_t *attr = myhtml_node_attribute_first(node);
  if(attr){
    while (attr) {
      const char *key = myhtml_attribute_key(attr, NULL);
      if(key) {
        const char *value = myhtml_attribute_value(attr, NULL);
        if(value){
          html_vec_str_t vec;
          html_vec_init(&vec);

          html_vec_push(&vec, html_string_copy(key));
          html_vec_push(&vec, html_string_copy(value));
          
          html_vec_push(&workspace->buffers, vec);

          int last = workspace->buffers.length - 1;
          html_vec_push(&indices, last);
        }
      }
      attr = myhtml_attribute_next(attr);
    } // while attr
  } // if attr

  if(indices.length == 0) {
    html_vec_deinit(&indices);
    return NULL;
  }

  return (html_vec_int_t*)&indices;
}