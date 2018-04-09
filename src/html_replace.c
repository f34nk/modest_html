#include "html_replace.h"

bool html_replace_node_with_collection(myhtml_tree_node_t* node, myhtml_collection_t* new_collection)
{
  if(node == NULL) {
    return false;
  }

  for(size_t i = 0; i < new_collection->length; i++) {
    myhtml_tree_node_t* new_node = new_collection->list[i];
    myhtml_node_insert_before(node, new_node);
  }
  myhtml_node_delete_recursive(node);

  return true;
}

bool html_replace(html_workspace_t* workspace, int collection_index, int new_collection_index)
{
  if(workspace == NULL) {
    fprintf(stderr, "html_replace() - Empty workspace.\n");
    return false;
  }

  myhtml_collection_t* collection = html_get_collection(workspace, collection_index);
  if(collection == NULL) {
    fprintf(stderr, "html_replace() - Empty collection\n");
    return false;
  }

  myhtml_collection_t* new_collection = html_get_collection(workspace, new_collection_index);
  if(new_collection == NULL) {
    fprintf(stderr, "html_replace() - Empty new collection\n");
    return false;
  }

  if(collection && collection->list && collection->length &&
      new_collection && new_collection->list && new_collection->length) {
    for(size_t i = 0; i < collection->length; i++) {
      myhtml_tree_node_t* node = collection->list[i];
      html_replace_node_with_collection(node, new_collection);
    }
  }

  return true;
}