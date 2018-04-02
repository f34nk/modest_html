#include "html_replace.h"

bool prepend_nodes(myhtml_tree_node_t *node, myhtml_collection_t *new_collection)
{
  if(node == NULL) {
    return false;
  }

  myhtml_tree_node_t *first_child = myhtml_node_child(node);
  myhtml_tree_node_t *next_child = (first_child) ? myhtml_node_next(first_child) : NULL;

  if(node && first_child) {
    for(size_t i = 0; i < new_collection->length; i++) {
      myhtml_tree_node_t *new_node = new_collection->list[i];
      myhtml_node_insert_before(first_child, new_node);
    }
  }
  else if(node && first_child == NULL && next_child == NULL) {
    for(size_t i = 0; i < new_collection->length; i++) {
      myhtml_tree_node_t *new_node = new_collection->list[i];
      myhtml_node_append_child(node, new_node);
    }
  }

  return true;
}

bool prepend_buffer(myhtml_tree_node_t *node, html_vec_t *new_buffer)
{
  if(node == NULL) {
    return false;
  }

  return true;
}

bool html_prepend_collection(html_workspace_t *workspace, int collection_index, int new_collection_index)
{
  if(workspace == NULL) {
    fprintf(stderr, "html_prepend_collection() - Empty workspace.\n");
    return false;
  }

  myhtml_collection_t *collection = html_get_collection(workspace, collection_index);
  if(collection == NULL) {
    fprintf(stderr, "html_prepend_collection() - Empty collection\n");
    return false;
  }

  myhtml_collection_t *new_collection = html_get_collection(workspace, new_collection_index);
  if(new_collection == NULL) {
    fprintf(stderr, "html_prepend_collection() - Empty new collection\n");
    return false;
  }
  
  if(collection && collection->list && collection->length &&
    new_collection && new_collection->list && new_collection->length) {
    for(size_t i = 0; i < collection->length; i++) {
      myhtml_tree_node_t *node = collection->list[i];
      prepend_nodes(node, new_collection);
    }
  }

  return true;
}

bool html_prepend_buffer(html_workspace_t *workspace, int collection_index, int new_buffer_index)
{
  if(workspace == NULL) {
    fprintf(stderr, "html_prepend_buffer() - Empty workspace.\n");
    return false;
  }

  myhtml_collection_t *collection = html_get_collection(workspace, collection_index);
  if(collection == NULL) {
    fprintf(stderr, "html_prepend_buffer() - Empty collection\n");
    return false;
  }

  html_vec_t *new_buffer = html_get_buffer(workspace, new_buffer_index);
  if(new_buffer == NULL) {
    fprintf(stderr, "html_prepend_buffer() - Empty new buffer\n");
    return false;
  }
  
  if(collection && collection->list && collection->length &&
    new_buffer->length) {
    for(size_t i = 0; i < collection->length; i++) {
      myhtml_tree_node_t *node = collection->list[i];
      prepend_buffer(node, new_buffer);
    }
  }

  return true;
}