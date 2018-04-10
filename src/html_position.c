#include "html_position.h"

bool html_equal_position(myhtml_tree_node_t* node, myhtml_tree_node_t* another_node)
{
  // myhtml_position_t myhtml_node_raw_position();
  myhtml_position_t a = myhtml_node_element_position(node);
  myhtml_position_t b = myhtml_node_element_position(another_node);
  if(a.begin == b.begin) {
    return true;
  }
  return false;
}

int html_get_position_for_node(myhtml_tree_node_t* node)
{
  myhtml_tree_node_t* parent = myhtml_node_parent(node);
  if(parent) {
    // check first child
    myhtml_tree_node_t* next = myhtml_node_child(parent);
    if(html_equal_position(node, next)) {
      return 1;
    }
    else {
      int position = 1;
      while(next) {
        next = myhtml_node_next(next);
        position += 1;
        if(html_equal_position(node, next)) {
          return position;
        }
      }
    }
  }
  return -1;
}

int html_position(html_workspace_t* workspace, int collection_index)
{
  if(workspace == NULL) {
    fprintf(stderr, "html_position() - Empty workspace.\n");
    return -1;
  }

  myhtml_collection_t* collection = html_get_collection(workspace, collection_index);
  if(collection == NULL) {
    fprintf(stderr, "html_position() - Empty collection\n");
    return -1;
  }

  if(collection && collection->list && collection->length) {

    html_vec_str_t buffer;
    html_vec_init(&buffer);

    for(size_t i = 0; i < collection->length; i++) {
      myhtml_tree_node_t* node = collection->list[i];
      if(node) {
        int pos = html_get_position_for_node(node);
        if(pos != -1) {
          char* data = (char*)html_malloc(1 + sizeof(pos));
          sprintf(data, "%d", pos);
          html_vec_push(&buffer, data);
        }
      }
    }

    html_vec_push(&workspace->buffers, buffer);

    return workspace->buffers.length - 1;
  }

  return -1;
}