#include "html_slice.h"

myhtml_collection_t* slice_collection(myhtml_collection_t* collection, int start, int end)
{
  if(end == -1) {
    end = collection->length;
  }

  if(start >= 0 && end >= 0) {

#ifdef MODEST_HTML_DEBUG
    printf("slice_collection()\n");
#endif

    int new_collection_size = end - start;
#ifdef MODEST_HTML_DEBUG
    printf("\tnew_collection_size %d\n", new_collection_size);
#endif
    mystatus_t status;
    myhtml_collection_t* new_collection = myhtml_collection_create(new_collection_size, &status);
    if(status != MyCORE_STATUS_OK) {
      fprintf(stderr, "slice_collection() - Failed to create new collection.\n");
      return NULL;
    }

    // range 0 to start
    for(size_t i = 0; i < start; i++) {
      myhtml_tree_node_t* node = collection->list[i];
      if(node) {
        myhtml_tag_id_t tag_id = myhtml_node_tag_id(node);
        const char* tag_name = myhtml_tag_name_by_id(node->tree, tag_id, NULL);
#ifdef MODEST_HTML_DEBUG
        printf("\texclude %d, tag_name %s\n", (int)i, tag_name);
#endif
      }
    }

    // range start to end
    for(size_t i = start; i < end; i++) {
      myhtml_tree_node_t* node = collection->list[i];
      if(node) {
        myhtml_tag_id_t tag_id = myhtml_node_tag_id(node);
        const char* tag_name = myhtml_tag_name_by_id(node->tree, tag_id, NULL);
#ifdef MODEST_HTML_DEBUG
        printf("\tinclude %d, tag_name %s\n", (int)i, tag_name);
#endif
        if(myhtml_collection_check_size(new_collection, 1, 1024) == MyHTML_STATUS_OK) {
          new_collection->list[new_collection->length] = node;
          new_collection->length++;
        }
      }
    }

    // range end to collection end
    for(size_t i = end; i < collection->length; i++) {
      myhtml_tree_node_t* node = collection->list[i];
      if(node) {
        myhtml_tag_id_t tag_id = myhtml_node_tag_id(node);
        const char* tag_name = myhtml_tag_name_by_id(node->tree, tag_id, NULL);
#ifdef MODEST_HTML_DEBUG
        printf("\texclude %d, tag_name %s\n", (int)i, tag_name);
#endif
      }
    }

    return new_collection;
  }

  return NULL;
}

int html_slice(html_workspace_t* workspace, int collection_index, int start, int end)
{
  if(workspace == NULL) {
    fprintf(stderr, "html_slice() - Empty workspace.\n");
    return -1;
  }

  myhtml_collection_t* collection = html_get_collection(workspace, collection_index);
  if(collection == NULL) {
    fprintf(stderr, "html_slice() - Empty collection\n");
    return -1;
  }

  if(collection && collection->list && collection->length) {
    myhtml_collection_t* new_collection = slice_collection(collection, start, end);
    if(new_collection && new_collection->list && new_collection->length) {
      html_vec_push(&workspace->collections, new_collection);
      return workspace->collections.length - 1;
    }
  }

  return -1;
}