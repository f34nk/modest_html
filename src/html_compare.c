#include "html_compare.h"
#include "html_compare_node.h"

// void add_set_attribute_instruction(html_workspace_t *workspace, const char* selector, const char* key, const char* value, html_vec_int_t* buffer_indices)
// {
//   html_vec_str_t vec;
//   html_vec_init(&vec);
//   char *copy;
//   html_string_copy("set_attribute", copy);
//   html_vec_push(&vec, copy);
//   char *selector_copy;
//   html_string_copy(selector, selector_copy);
//   html_vec_push(&vec, selector_copy);
//   char *key_copy;
//   html_string_copy(key, key_copy);
//   html_vec_push(&vec, key_copy);
//   char *value_copy;
//   html_string_copy(value, value_copy);
//   html_vec_push(&vec, value_copy);
//   html_vec_push(&workspace->buffers, vec);
//   html_vec_push(buffer_indices, workspace->buffers.length - 1);
// }

// void add_set_text_instruction(html_workspace_t *workspace, const char* selector, const char* text, html_vec_int_t* buffer_indices)
// {
//   html_vec_str_t vec;
//   html_vec_init(&vec);
//   char *copy;
//   html_string_copy("set_text", copy);
//   html_vec_push(&vec, copy);
//   char *selector_copy;
//   html_string_copy(selector, selector_copy);
//   html_vec_push(&vec, selector_copy);
//   char *text_copy;
//   html_string_copy(text, text_copy);
//   html_vec_push(&vec, text_copy);
//   html_vec_push(&workspace->buffers, vec);
//   html_vec_push(buffer_indices, workspace->buffers.length - 1);
// }

// void add_set_tag_instruction(html_workspace_t *workspace, const char* selector, const char* tag_name, html_vec_int_t* buffer_indices)
// {
//   html_vec_str_t vec;
//   html_vec_init(&vec);
//   char *copy;
//   html_string_copy("set_tag", copy);
//   html_vec_push(&vec, copy);
//   char *selector_copy;
//   html_string_copy(selector, selector_copy);
//   html_vec_push(&vec, selector_copy);
//   char *tag_name_copy;
//   html_string_copy(tag_name, tag_name_copy);
//   html_vec_push(&vec, tag_name_copy);
//   html_vec_push(&workspace->buffers, vec);
//   html_vec_push(buffer_indices, workspace->buffers.length - 1);
// }

// bool compare_attributes_with_index(html_workspace_t *workspace, int attribute1_index, int attribute2_index) 
// {
//   html_vec_str_t *attributes1 = html_get_buffer(workspace, attribute1_index);
//   html_vec_str_t *attributes2 = html_get_buffer(workspace, attribute2_index);

//   char *key1 = attributes1->data[0];
//   char *value1 = attributes1->data[1];

//   char *key2 = attributes2->data[0];
//   char *value2 = attributes2->data[1];

//   if(strcmp(key1, key2) == 0 && strcmp(value1, value2) == 0) {
//     return true;
//   }
//   return false;
// }



  /*

void compare_nodes(html_workspace_t *workspace, myhtml_tree_node_t *node1, myhtml_tree_node_t *node2, int indent, html_vec_int_t *buffer_indices)
{
#ifdef MODEST_HTML_DEBUG
    for(int i = 0; i < indent; i++){
      printf(".");
    }
#endif

  if(!node1 && node2) {
    // is missing in html1
    // append to html1
    myhtml_tree_node_t* parent_node2 = myhtml_node_parent(node2);

    char *selector2 = html_serialize_selector(parent_node2);
    // remove_scope_from_selector(selector2, scope);
    char *html = html_serialize_node(node2);

#ifdef MODEST_HTML_DEBUG
    printf("append '%s' to '%s'\n", html, selector2);
#endif
    // add_append_instruction(result, selector2, html);

    html_free(selector2);
    html_free(html);
  }
  else if(node1 && !node2) {
    // is missing in html2
    // remove from html1
    // myhtml_tree_node_t* parent_node1 = myhtml_node_parent(node1);
    // const char *parent_tag_name1 = myhtml_tag_name_by_id(node1->tree, myhtml_node_tag_id(parent_node1), NULL);
    // printf("remove '%s' from '%s' in html1\n", tag_name1, parent_tag_name1);

    char *selector1 = html_serialize_selector(node1);
    // remove_scope_from_selector(selector1, scope);
    char *html = html_serialize_node(node1);

#ifdef MODEST_HTML_DEBUG
    printf("remove '%s' at '%s'\n", html, selector1);
#endif
    // add_remove_instruction(result, selector1);

    html_free(selector1);
    html_free(html);
  }
  else if(!node1 && !node2){
#ifdef MODEST_HTML_DEBUG
    printf("NULL NULL\n");
#endif
  }
  else {
    if(compare_tag_names(node1, node2) == true){
      if(html_node_is_text(node1) && html_node_is_text(node2)){
        if(compare_text(node1, node2) == false){

          // myhtml_tree_node_t* parent_node1 = myhtml_node_parent(node1);
          // char *selector1 = serialize_selector(parent_node1);
          // remove_scope_from_selector(selector1, scope);

          myhtml_tree_node_t* parent_node2 = myhtml_node_parent(node2);
          char *selector2 = html_serialize_selector(parent_node2);
          // remove_scope_from_selector(selector2, scope);

          mycore_string_t *string2 = myhtml_node_string(node2);
          char *data2 = mycore_string_data(string2);
#ifdef MODEST_HTML_DEBUG
          printf("set_text '%s' to '%s'\n", data2, selector2);
#endif
          add_set_text_instruction(workspace, selector2, data2, buffer_indices);

          // free(selector1);
          html_free(selector2);
        }
      }
      else if (compare_attributes(workspace, node1, node2, buffer_indices) == true) {
        const char *tag_name1 = myhtml_tag_name_by_id(node1->tree, myhtml_node_tag_id(node1), NULL);
        const char *tag_name2 = myhtml_tag_name_by_id(node2->tree, myhtml_node_tag_id(node2), NULL);
#ifdef MODEST_HTML_DEBUG
        printf("'%s' == '%s'\n", tag_name1, tag_name2);
#endif
      }
    }
    else {
      char *selector1 = html_serialize_selector(node1);
      // remove_scope_from_selector(selector1, scope);
      // char *selector2 = serialize_selector(node2);
      // remove_scope_from_selector(selector2, scope);
      const char *tag_name2 = myhtml_tag_name_by_id(node2->tree, myhtml_node_tag_id(node2), NULL);
#ifdef MODEST_HTML_DEBUG
      printf("set_tag '%s' for '%s'\n", tag_name2, selector1);
#endif
      add_set_tag_instruction(workspace, selector1, tag_name2, buffer_indices);

      html_free(selector1);
      // free(selector2);
    }
  } // else
}
*/

void compare_trees(html_workspace_t *workspace, myhtml_tree_node_t *node1, myhtml_tree_node_t *node2, int indent, html_vec_int_t *buffer_indices)
{
  myhtml_tree_node_t* child_node1 = NULL;
  myhtml_tree_node_t* child_node2 = NULL;
  myhtml_tree_node_t* next_node1 = NULL;
  myhtml_tree_node_t* next_node2 = NULL;

  if(node1){
    child_node1 = myhtml_node_child(node1);
    next_node1 = myhtml_node_next(node1);
  }
  if(node2){
    child_node2 = myhtml_node_child(node2);
    next_node2 = myhtml_node_next(node2);
  }

  html_compare_nodes(workspace, node1, node2, indent, buffer_indices);

  if(child_node1 && child_node2) {
  // if(child_node1 || child_node2) {
    compare_trees(workspace, child_node1, child_node2, indent + 1, buffer_indices);
  }

  if(next_node1 || next_node2){
    compare_trees(workspace, next_node1, next_node2, indent, buffer_indices);
  }
}

bool html_compare(html_workspace_t *workspace, int collection1_index, int collection2_index, html_vec_int_t *buffer_indices)
{
  if(workspace == NULL) {
    fprintf(stderr, "html_compare() - Empty workspace.\n");
    return false;
  }
  
  myhtml_collection_t *collection1 = html_get_collection(workspace, collection1_index);
  if(collection1 == NULL) {
    fprintf(stderr, "html_compare() - Empty collection1\n");
    return false;
  }

  myhtml_collection_t *collection2 = html_get_collection(workspace, collection2_index);
  if(collection2 == NULL) {
    fprintf(stderr, "html_compare() - Empty collection2\n");
    return false;
  }

  if(collection1 && collection1->list && collection1->length &&
     collection2 && collection2->list && collection2->length) {
    
    myhtml_tree_node_t *node1 = collection1->list[0];
    myhtml_tree_node_t *node2 = collection2->list[0];

    compare_trees(workspace, node1, node2, 0, buffer_indices);

    return (buffer_indices->length > 0) ? true : false;
  }

  return false;
}