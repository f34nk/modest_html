#include "html_append.h"

bool append_nodes(myhtml_tree_node_t *node, myhtml_collection_t *new_collection)
{
  if(node == NULL) {
    return false;
  }

//   myhtml_tree_node_t *last_child = myhtml_node_last_child(node);
//   myhtml_tree_node_t *prev_child = (last_child) ? myhtml_node_prev(last_child) : NULL;

//   if(node && last_child) {
//     for(size_t i = 0; i < new_collection->length; i++) {
//       myhtml_tree_node_t *new_node = new_collection->list[i];

//       if(i == 0) {
// #ifdef MODEST_HTML_DEBUG
//         printf("append_nodes() - insert after\n");
// #endif
//         myhtml_node_insert_after(last_child, new_node);
//         last_child = new_node;
//       }
//       else {
// #ifdef MODEST_HTML_DEBUG
//         printf("append_nodes() - append child\n");
// #endif
//         // myhtml_node_append_child(node, new_node);
//         myhtml_node_insert_after(last_child, new_node);
//         last_child = new_node;
//       }
//     }
//   }
//   else if(node && last_child == NULL && prev_child == NULL) {
    for(size_t i = 0; i < new_collection->length; i++) {
      myhtml_tree_node_t *new_node = new_collection->list[i];
// #ifdef MODEST_HTML_DEBUG
//       printf("append_nodes() - append child %d\n", i);
//       html_dump_node(stdout, node);
//       html_dump_node(stdout, new_node);
// #endif
      myhtml_node_append_child(node, new_node);
// #ifdef MODEST_HTML_DEBUG
//       printf("append_nodes() - append child %d\n", i);
//       html_dump_node(stdout, node);
// #endif
    }
  // }

  return true;
}

bool append_buffer(myhtml_tree_node_t *node, html_vec_t *new_buffer)
{
  if(node == NULL) {
    return false;
  }

  return true;
}

bool html_append_collection(html_workspace_t *workspace, int collection_index, int new_collection_index)
{
  if(workspace == NULL) {
    fprintf(stderr, "html_append_collection() - Empty workspace.\n");
    return false;
  }

  myhtml_collection_t *collection = html_get_collection(workspace, collection_index);
  if(collection == NULL) {
    fprintf(stderr, "html_append_collection() - Empty collection\n");
    return false;
  }

  myhtml_collection_t *new_collection = html_get_collection(workspace, new_collection_index);
  if(new_collection == NULL) {
    fprintf(stderr, "html_append_collection() - Empty new collection\n");
    return false;
  }
  
  if(collection && collection->list && collection->length &&
    new_collection && new_collection->list && new_collection->length) {

    if(collection->length > 1) {
      fprintf(stderr, "html_append_collection() - Not supported.\n");
      return false;
    }
    else {
      for(size_t i = 0; i < collection->length; i++) {
        myhtml_tree_node_t *node = collection->list[i];
        append_nodes(node, new_collection);
      }
    }
    

//     if(collection->length == 1 && new_collection->length == 1) {
//       myhtml_tree_node_t *node = collection->list[0];
//       append_nodes(node, new_collection);
//     }
//     else {
// #ifdef MODEST_HTML_DEBUG
//       printf("html_append_collection() - Not supported (%d, %d)\n", collection->length, new_collection->length);
// #endif
//       for(size_t i = 0; i < collection->length; i++) {
//         myhtml_tree_node_t *node = collection->list[i];
//         append_nodes(node, new_collection);
//       }
//     }
  }

  return true;
}

bool html_append_buffer(html_workspace_t *workspace, int collection_index, int new_buffer_index)
{
  if(workspace == NULL) {
    fprintf(stderr, "html_append_buffer() - Empty workspace.\n");
    return false;
  }

  myhtml_collection_t *collection = html_get_collection(workspace, collection_index);
  if(collection == NULL) {
    fprintf(stderr, "html_append_buffer() - Empty collection\n");
    return false;
  }

  html_vec_t *new_buffer = html_get_buffer(workspace, new_buffer_index);
  if(new_buffer == NULL) {
    fprintf(stderr, "html_append_buffer() - Empty new buffer\n");
    return false;
  }
  
  if(collection && collection->list && collection->length &&
    new_buffer->length) {

    for(size_t i = 0; i < collection->length; i++) {
      myhtml_tree_node_t *node = collection->list[i];
      append_buffer(node, new_buffer);
    }
    // if(collection->length == 1) {
    //   myhtml_tree_node_t *node = collection->list[0];
    //   append_buffer(node, new_buffer);
    // }
    // else {
    //   for(size_t i = 0; i < collection->length; i++) {
    //     myhtml_tree_node_t *node = collection->list[i];
    //     append_buffer(node, new_buffer);
    //   }
    // }
  }

  return true;
}