#include "html_insert_after.h"

bool html_insert_collection_after_node(myhtml_tree_node_t* node, myhtml_collection_t* new_collection)
{
  if(node == NULL) {
    return false;
  }

  for(size_t i = new_collection->length; i-- > 0;) {
    myhtml_tree_node_t* new_node = new_collection->list[i];
    myhtml_node_insert_after(node, new_node);
  }

  return true;
}

bool html_insert_after(html_workspace_t* workspace, int collection_index, int new_collection_index)
{
  if(workspace == NULL) {
#ifdef MODEST_HTML_DEBUG
    fprintf(stderr, "html_insert_after() - Empty workspace.\n");
#endif
    return false;
  }

  myhtml_collection_t* collection = html_get_collection(workspace, collection_index);
  if(collection == NULL) {
#ifdef MODEST_HTML_DEBUG
    fprintf(stderr, "html_insert_after() - Empty collection\n");
#endif
    return false;
  }

  myhtml_collection_t* new_collection = html_get_collection(workspace, new_collection_index);
  if(new_collection == NULL) {
#ifdef MODEST_HTML_DEBUG
    fprintf(stderr, "html_insert_after() - Empty new collection\n");
#endif
    return false;
  }

  if(collection && collection->list && collection->length &&
      new_collection && new_collection->list && new_collection->length) {
    if(collection->length > 1){
      fprintf(stderr, "html_insert_after() - Only single selected node allowed.\n");
      return false;
    }
    else {
      myhtml_tree_node_t* node = collection->list[0];
      html_insert_collection_after_node(node, new_collection);
      return true;
    }
    // for(size_t i = 0; i < collection->length; i++) {
    //   myhtml_tree_node_t* node = collection->list[i];
    //   html_insert_collection_before_node(node, new_collection);
    // }
  }
  
  // if(collection && collection->list && collection->length &&
  //     new_collection && new_collection->list && new_collection->length) {
  //   for(size_t i = 0; i < collection->length; i++) {
  //     myhtml_tree_node_t* node = collection->list[i];
  //     html_insert_collection_after_node(node, new_collection);
  //   }
  // }

  return false;
}