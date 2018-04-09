#include "html_compare_node.h"
#include "html_serialize.h"
#include "html_node.h"
#include "html_compare_instructions.h"

bool html_compare_tag_names(html_node_t* node1, html_node_t* node2)
{
  if(node1->tag_name == NULL || node2->tag_name == NULL) {
    return false;
  }
  return (strcmp(node1->tag_name, node2->tag_name) == 0) ? true : false;
}

bool html_compare_text(html_node_t* node1, html_node_t* node2)
{
  if(node1->text == NULL && node2->text == NULL) {
    return true;
  }
  if(node1->text == NULL || node2->text == NULL) {
    return false;
  }
  return (strcmp(node1->text, node2->text) == 0) ? true : false;
}

void html_compare_attribute_with_index(html_workspace_t* workspace, html_node_t* node1, html_node_t* node2, int index,  html_vec_int_t* buffer_indices)
{
  char* key1 = html_node_key_for_index(node1, index);
  char* value1 = html_node_value_for_key(node1, key1);
  char* key2 = html_node_key_for_index(node2, index);
  char* value2 = html_node_value_for_key(node2, key2);
  if(strcmp(key1, key2) != 0) {
    add_set_attribute_instruction(workspace, node2->selector, key2, value2, buffer_indices);
  }
  else if(strcmp(key1, key2) == 0 && strcmp(value1, value2) != 0) {
    add_set_attribute_instruction(workspace, node2->selector, key2, value2, buffer_indices);
  }
}

void html_compare_node_attributes(html_workspace_t* workspace, html_node_t* node1, html_node_t* node2, html_vec_int_t* buffer_indices)
{
  if(html_node_attributes_count(node1) == html_node_attributes_count(node2)) {
    for(int i = 0; i < html_node_attributes_count(node1); i++) {
      html_compare_attribute_with_index(workspace, node1, node2, i, buffer_indices);
    }
  }
  else if(html_node_attributes_count(node1) > html_node_attributes_count(node2)) {
    for(int i = 0; i < html_node_attributes_count(node1); i++) {
      if(i < html_node_attributes_count(node2)) {
        html_compare_attribute_with_index(workspace, node1, node2, i, buffer_indices);
      }
      else {
        // this attr is missing in node2, remove it
        char* key1 = html_node_key_for_index(node1, i);
        add_set_attribute_instruction(workspace, node1->selector, key1, "", buffer_indices);
      }
    }
  }
  else if(html_node_attributes_count(node1) < html_node_attributes_count(node2)) {
    for(int i = 0; i < html_node_attributes_count(node2); i++) {
      if(i < html_node_attributes_count(node1)) {
        html_compare_attribute_with_index(workspace, node1, node2, i, buffer_indices);
      }
      else {
        // this attr is missing in node1, add it
        char* key2 = html_node_key_for_index(node2, i);
        char* value2 = html_node_value_for_key(node2, key2);
        add_set_attribute_instruction(workspace, node2->selector, key2, value2, buffer_indices);
      }
    }
  }
}

void html_compare_node_params(html_workspace_t* workspace, html_node_t* node1, html_node_t* node2, html_vec_int_t* buffer_indices)
{
  if(html_compare_tag_names(node1, node2) == false) {
    add_set_tag_instruction(workspace, node1->selector, node2->tag_name, buffer_indices);
  }

  if(html_node_is_text(node1) && html_node_is_text(node2)) {
    // this is a text node
    if(html_compare_text(node1, node2) == false) {
      // different text
      add_set_text_instruction(workspace, node2->parent_selector, node2->text, buffer_indices);
    }
  }
  else if(html_node_has_attributes(node1) && html_node_has_attributes(node2)) {
    html_compare_node_attributes(workspace, node1, node2, buffer_indices);
  }

}

void html_compare_nodes(html_workspace_t* workspace, myhtml_tree_node_t* node1, myhtml_tree_node_t* node2, int indent, html_vec_int_t* buffer_indices)
{
#ifdef MODEST_HTML_DEBUG
  for(int i = 0; i < indent; i++) {
    printf(".");
  }
#endif

  if(!node1 && node2) {
    add_append_instruction(workspace, node2, buffer_indices);
  }
  else if(node1 && !node2) {
    add_remove_instruction(workspace, node1, buffer_indices);
  }
  else if(!node1 && !node2) {
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

    html_compare_node_params(workspace, &params1, &params2, buffer_indices);

    html_node_destroy(&params1);
    html_node_destroy(&params2);
  }
}