#include "html_replace.h"

bool html_wrap_node(myhtml_tree_node_t* node, myhtml_tree_node_t* new_node)
{
  if(node == NULL) {
    return false;
  }
  if(new_node == NULL) {
    return false;
  }

  myhtml_node_insert_before(node, new_node);
  myhtml_tree_node_t* detached = myhtml_node_remove(node);
  myhtml_node_append_child(new_node, detached);

  return true;
}

bool html_wrap_collection(myhtml_collection_t* collection, myhtml_tree_node_t* new_node)
{
  if(collection == NULL) {
    return false;
  }
  if(new_node == NULL) {
    return false;
  }

  myhtml_tree_node_t* first_node = collection->list[0];
  myhtml_node_insert_before(first_node, new_node);
  for(size_t i = 0; i < collection->length; i++) {
    myhtml_tree_node_t* node = collection->list[i];
    myhtml_tree_node_t* detached = myhtml_node_remove(node);
    myhtml_node_append_child(new_node, detached);
  }

  return true;
}

bool html_wrap(html_workspace_t* workspace, int collection_index, int new_collection_index)
{
  if(workspace == NULL) {
    fprintf(stderr, "html_wrap() - Empty workspace.\n");
    return false;
  }

  myhtml_collection_t* collection = html_get_collection(workspace, collection_index);
  if(collection == NULL) {
    fprintf(stderr, "html_wrap() - Empty collection\n");
    return false;
  }

  myhtml_collection_t* new_collection = html_get_collection(workspace, new_collection_index);
  if(new_collection == NULL) {
    fprintf(stderr, "html_wrap() - Empty new collection\n");
    return false;
  }

  if(collection && collection->list && collection->length &&
     new_collection && new_collection->list && new_collection->length) {

    if(collection->length == 1) {
      myhtml_tree_node_t* node = collection->list[0];
      myhtml_tree_node_t* new_node = new_collection->list[0];
      html_wrap_node(node, new_node);
      return true;
    }
    else {
      myhtml_tree_node_t* new_node = new_collection->list[0];
      html_wrap_collection(collection, new_node);
    }
  }

  return false;
}