#include "html_insert_before.h"

bool html_insert_collection_before_node(myhtml_tree_node_t* node, myhtml_collection_t* new_collection)
{
  if(node == NULL) {
    return false;
  }

  for(size_t i = 0; i < new_collection->length; i++) {
    myhtml_tree_node_t* new_node = new_collection->list[i];
    myhtml_node_insert_before(node, new_node);
  }

  return true;
}

bool html_insert_before(html_workspace_t* workspace, int collection_index, int new_collection_index)
{
  if(workspace == NULL) {
    fprintf(stderr, "html_insert_before() - Empty workspace.\n");
    return false;
  }

  myhtml_collection_t* collection = html_get_collection(workspace, collection_index);
  if(collection == NULL) {
    fprintf(stderr, "html_insert_before() - Empty collection\n");
    return false;
  }

  myhtml_collection_t* new_collection = html_get_collection(workspace, new_collection_index);
  if(new_collection == NULL) {
    fprintf(stderr, "html_insert_before() - Empty new collection\n");
    return false;
  }

  if(collection && collection->list && collection->length &&
      new_collection && new_collection->list && new_collection->length) {
    if(collection->length > 1){
      fprintf(stderr, "html_insert_before() - Only single selected node allowed.\n");
      return false;
    }
    else {
      myhtml_tree_node_t* node = collection->list[0];
      html_insert_collection_before_node(node, new_collection);
    }
    // for(size_t i = 0; i < collection->length; i++) {
    //   myhtml_tree_node_t* node = collection->list[i];
    //   html_insert_collection_before_node(node, new_collection);
    // }
  }

  return true;
}