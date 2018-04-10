#include "html_remove.h"

bool html_remove(html_workspace_t* workspace, int collection_index)
{
  if(workspace == NULL) {
#ifdef MODEST_HTML_DEBUG
    fprintf(stderr, "html_remove() - Empty workspace.\n");
#endif
    return false;
  }

  myhtml_collection_t* collection = html_get_collection(workspace, collection_index);
  if(collection == NULL) {
#ifdef MODEST_HTML_DEBUG
    fprintf(stderr, "html_remove() - Empty collection\n");
#endif
    return false;
  }

  if(collection && collection->list && collection->length) {
    for(size_t i = 0; i < collection->length; i++) {
      myhtml_tree_node_t* node = collection->list[i];
      if(node) {
        myhtml_node_delete_recursive(node);
      }
    }
  }

  return true;
}