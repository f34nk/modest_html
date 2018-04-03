#include "html_compare.h"
#include "html_compare_node.h"

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