#include "html_compare_node.h"
#include "html_serialize.h"
#include "html_node.h"

// /*

// "id=b class=a"


// list of keys
// id, class

// map of key values
// id=b
// class=a

// sort list of keys
// class, id

// ---

// compare list of keys

// if same length
//   if same key
//     if val2 is diff
//       set val1 = val2
//   if diff key
//     remove attr1
//     add attr2
// if length1 < lenght2
//   compare until same lenght1
//   i=length1 until lenght2
//     add attr2
// if length1 > lenght2
//   compare until same lenght2
//   i=length2 until length1
//     remove attr1
    
//  */

void add_append_instruction(html_workspace_t *workspace, myhtml_tree_node_t *node, html_vec_int_t *buffer_indices)
{
  myhtml_tree_node_t* parent_node = myhtml_node_parent(node);

  char *selector = html_serialize_selector(parent_node);
  char *html = html_serialize_node(node);

#ifdef MODEST_HTML_DEBUG
  printf("append '%s' to '%s'\n", html, selector);
#endif

  html_vec_str_t vec;
  html_vec_init(&vec);

  char *copy;
  html_string_copy("append", copy);
  html_vec_push(&vec, copy);

  html_vec_push(&vec, selector);
  html_vec_push(&vec, html);

  html_vec_push(&workspace->buffers, vec);
  html_vec_push(buffer_indices, workspace->buffers.length - 1);
}

void add_remove_instruction(html_workspace_t *workspace, myhtml_tree_node_t *node, html_vec_int_t *buffer_indices)
{
  char *selector = html_serialize_selector(node);

#ifdef MODEST_HTML_DEBUG
    printf("remove '%s'\n", selector);
#endif

  html_vec_str_t vec;
  html_vec_init(&vec);

  char *copy;
  html_string_copy("remove", copy);
  html_vec_push(&vec, copy);

  html_vec_push(&vec, selector);

  html_vec_push(&workspace->buffers, vec);
  html_vec_push(buffer_indices, workspace->buffers.length - 1);
}

void add_set_attribute_instruction(html_workspace_t *workspace, const char* selector, const char* key, const char* value, html_vec_int_t* buffer_indices)
{
#ifdef MODEST_HTML_DEBUG
  printf("set_attribute '%s = %s' for '%s'\n", key, value, selector);
#endif

  html_vec_str_t vec;
  html_vec_init(&vec);

  char *copy;
  html_string_copy("set_attribute", copy);
  html_vec_push(&vec, copy);
  
  char *selector_copy;
  html_string_copy(selector, selector_copy);
  html_vec_push(&vec, selector_copy);
  
  char *key_copy;
  html_string_copy(key, key_copy);
  html_vec_push(&vec, key_copy);
  
  char *value_copy;
  html_string_copy(value, value_copy);
  html_vec_push(&vec, value_copy);
  
  html_vec_push(&workspace->buffers, vec);
  html_vec_push(buffer_indices, workspace->buffers.length - 1);
}

void add_set_text_instruction(html_workspace_t *workspace, const char* selector, const char* text, html_vec_int_t* buffer_indices)
{
#ifdef MODEST_HTML_DEBUG
  printf("set_text '%s' for '%s'\n", text, selector);
#endif

  html_vec_str_t vec;
  html_vec_init(&vec);
  
  char *copy;
  html_string_copy("set_text", copy);
  html_vec_push(&vec, copy);
  
  char *selector_copy;
  html_string_copy(selector, selector_copy);
  html_vec_push(&vec, selector_copy);
  
  char *text_copy;
  html_string_copy(text, text_copy);
  html_vec_push(&vec, text_copy);
  
  html_vec_push(&workspace->buffers, vec);
  html_vec_push(buffer_indices, workspace->buffers.length - 1);
}

void add_set_tag_instruction(html_workspace_t *workspace, const char* selector, const char* tag_name, html_vec_int_t* buffer_indices)
{
#ifdef MODEST_HTML_DEBUG
  printf("set_tag '%s' for '%s'\n", tag_name, selector);
#endif

  html_vec_str_t vec;
  html_vec_init(&vec);
  char *copy;
  html_string_copy("set_tag", copy);
  html_vec_push(&vec, copy);
  char *selector_copy;
  html_string_copy(selector, selector_copy);
  html_vec_push(&vec, selector_copy);
  char *tag_name_copy;
  html_string_copy(tag_name, tag_name_copy);
  html_vec_push(&vec, tag_name_copy);
  html_vec_push(&workspace->buffers, vec);
  html_vec_push(buffer_indices, workspace->buffers.length - 1);
}

bool compare_tag_names(html_node_t *node1, html_node_t *node2)
{
  if(node1->tag_name == NULL || node2->tag_name == NULL) {
    return false;
  }
  return (strcmp(node1->tag_name, node2->tag_name) == 0) ? true : false;
}

bool compare_text(html_node_t *node1, html_node_t *node2)
{
  if(node1->text == NULL && node2->text == NULL) {
    return true;
  }
  if(node1->text == NULL || node2->text == NULL) {
    return false;
  }
  return (strcmp(node1->text, node2->text) == 0) ? true : false;
}

void compare_attributes(html_workspace_t *workspace, html_node_t *node1, html_node_t *node2, html_vec_int_t *buffer_indices)
{

}

void compare_node_params(html_workspace_t *workspace, html_node_t *node1, html_node_t *node2, html_vec_int_t *buffer_indices)
{
  if(compare_tag_names(node1, node2) == false) {
    add_set_tag_instruction(workspace, node1->selector, node2->tag_name, buffer_indices);
  }

  if(html_node_is_text(node1) && html_node_is_text(node2)) {
    // this is a text node
    if(compare_text(node1, node2) == false) {
      // different text
      add_set_text_instruction(workspace, node2->parent_selector, node2->text, buffer_indices);
    }
  }
  else if(html_node_has_attributes(node1) && html_node_has_attributes(node2)){
    compare_attributes(workspace, node1, node2, buffer_indices);
  }
  
}

void html_compare_nodes(html_workspace_t *workspace, myhtml_tree_node_t *node1, myhtml_tree_node_t *node2, int indent, html_vec_int_t *buffer_indices)
{
#ifdef MODEST_HTML_DEBUG
  for(int i = 0; i < indent; i++){
    printf(".");
  }
#endif

  if(!node1 && node2) {
    add_append_instruction(workspace, node2, buffer_indices);
  }
  else if(node1 && !node2) {
    add_remove_instruction(workspace, node1, buffer_indices);
  }
  else if(!node1 && !node2){
#ifdef MODEST_HTML_DEBUG
    printf("nothing to compare\n");
#endif
  }
  else {
    html_node_t params1;
    html_node_t params2;
    html_node_init(&params1);
    html_node_init(&params2);

    html_node_get(node1, &params1);
    html_node_get(node2, &params2);

    compare_node_params(workspace, &params1, &params2, buffer_indices);

    html_node_destroy(&params1);
    html_node_destroy(&params2);
  }
}