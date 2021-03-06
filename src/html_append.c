#include "html_append.h"

bool html_append_collection_to_node(myhtml_tree_node_t* node, myhtml_collection_t* new_collection)
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
  //         printf("html_append_collection_to_node() - insert after\n");
  // #endif
  //         myhtml_node_insert_after(last_child, new_node);
  //         last_child = new_node;
  //       }
  //       else {
  // #ifdef MODEST_HTML_DEBUG
  //         printf("html_append_collection_to_node() - append child\n");
  // #endif
  //         // myhtml_node_append_child(node, new_node);
  //         myhtml_node_insert_after(last_child, new_node);
  //         last_child = new_node;
  //       }
  //     }
  //   }
  //   else if(node && last_child == NULL && prev_child == NULL) {
  for(size_t i = 0; i < new_collection->length; i++) {
    myhtml_tree_node_t* new_node = new_collection->list[i];
    // #ifdef MODEST_HTML_DEBUG
    //       printf("html_append_collection_to_node() - append child %d\n", i);
    //       html_serialize_dump(stdout, node);
    //       html_serialize_dump(stdout, new_node);
    // #endif
    myhtml_node_append_child(node, new_node);
    // #ifdef MODEST_HTML_DEBUG
    //       printf("html_append_collection_to_node() - append child %d\n", i);
    //       html_serialize_dump(stdout, node);
    // #endif
  }
  // }

  return true;
}

bool html_append_buffer_to_node(myhtml_tree_node_t* node, html_vec_str_t* new_buffer)
{
  if(node == NULL) {
    return false;
  }

  return true;
}

bool html_append_collection(html_workspace_t* workspace, int collection_index, int new_collection_index)
{
  if(workspace == NULL) {
#ifdef MODEST_HTML_DEBUG
    fprintf(stderr, "html_append_collection() - Empty workspace.\n");
#endif
    return false;
  }

  myhtml_collection_t* collection = html_get_collection(workspace, collection_index);
  if(collection == NULL) {
#ifdef MODEST_HTML_DEBUG
    fprintf(stderr, "html_append_collection() - Empty collection\n");
#endif
    return false;
  }

  myhtml_collection_t* new_collection = html_get_collection(workspace, new_collection_index);
  if(new_collection == NULL) {
#ifdef MODEST_HTML_DEBUG
    fprintf(stderr, "html_append_collection() - Empty new collection\n");
#endif
    return false;
  }

  if(collection && collection->list && collection->length &&
      new_collection && new_collection->list && new_collection->length) {

    if(collection->length > 1) {
      fprintf(stderr, "html_append_collection() - Only single selected node allowed.\n");

      // TODO: implement append to multiple selected nodes

      // for(size_t i = 0; i < collection->length; i++) {
      //   myhtml_tree_node_t *node = collection->list[i];
      //   html_append_collection_to_node(node, new_collection);
      // }
      return false;
    }
    else {
      myhtml_tree_node_t* node = collection->list[0];
      html_append_collection_to_node(node, new_collection);
    }


    //     if(collection->length == 1 && new_collection->length == 1) {
    //       myhtml_tree_node_t *node = collection->list[0];
    //       html_append_collection_to_node(node, new_collection);
    //     }
    //     else {
    // #ifdef MODEST_HTML_DEBUG
    //       printf("html_append_collection() - Not supported (%d, %d)\n", collection->length, new_collection->length);
    // #endif
    //       for(size_t i = 0; i < collection->length; i++) {
    //         myhtml_tree_node_t *node = collection->list[i];
    //         html_append_collection_to_node(node, new_collection);
    //       }
    //     }
  }

  return true;
}

bool html_append_buffer(html_workspace_t* workspace, int collection_index, int new_buffer_index)
{
  if(workspace == NULL) {
#ifdef MODEST_HTML_DEBUG
    fprintf(stderr, "html_append_buffer() - Empty workspace.\n");
#endif
    return false;
  }

  myhtml_collection_t* collection = html_get_collection(workspace, collection_index);
  if(collection == NULL) {
#ifdef MODEST_HTML_DEBUG
    fprintf(stderr, "html_append_buffer() - Empty collection\n");
#endif
    return false;
  }

  html_vec_str_t* new_buffer = html_get_buffer(workspace, new_buffer_index);
  if(new_buffer == NULL) {
#ifdef MODEST_HTML_DEBUG
    fprintf(stderr, "html_append_buffer() - Empty new buffer\n");
#endif
    return false;
  }

  if(collection && collection->list && collection->length &&
      new_buffer->length) {

    for(size_t i = 0; i < collection->length; i++) {
      myhtml_tree_node_t* node = collection->list[i];
      html_append_buffer_to_node(node, new_buffer);
    }
    // if(collection->length == 1) {
    //   myhtml_tree_node_t *node = collection->list[0];
    //   html_append_buffer_to_node(node, new_buffer);
    // }
    // else {
    //   for(size_t i = 0; i < collection->length; i++) {
    //     myhtml_tree_node_t *node = collection->list[i];
    //     html_append_buffer_to_node(node, new_buffer);
    //   }
    // }
    return true;
  }

  return false;
}