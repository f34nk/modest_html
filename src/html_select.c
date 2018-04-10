#include "html_select.h"
#include "html_serialize.h"

mystatus_t html_serialization_bad_selectors(const char* data, size_t len, void* ctx)
{
  fprintf(stderr, "%.*s", (int)len, data);
  return MyCORE_STATUS_OK;
}

int html_prepare_selector(html_workspace_t* workspace, const char* selector, size_t selector_size)
{
  if(workspace == NULL) {
    fprintf(stderr, "html_prepare_selector() - Empty workspace.\n");
    return -1;
  }

  mycss_t* mycss = workspace->mycss;

  // init current entry
  mycss_entry_t* entry = mycss_entry_create();
  mystatus_t status = mycss_entry_init(mycss, entry);
  if(status != MyCORE_STATUS_OK) {
    fprintf(stderr, "html_prepare_selector() - Failed to init css entry\n");
    return -1;
  }

  mycss_selectors_list_t* list = mycss_selectors_parse(mycss_entry_selectors(entry), MyENCODING_UTF_8, selector, selector_size, &status);

  // check parsing errors
  if(list == NULL || (list->flags & MyCSS_SELECTORS_FLAGS_SELECTOR_BAD)) {
    fprintf(stderr, "html_prepare_selector() - Bad CSS Selectors\n");
    if(list) {
      mycss_selectors_serialization_list(mycss_entry_selectors(entry), list, html_serialization_bad_selectors, NULL);
      fprintf(stderr, "\n");
    }
    return -1;
  }

  html_vec_push(&workspace->entries, entry);
  html_vec_push(&workspace->selectors, list);

  return workspace->selectors.length - 1;
}

int html_select(html_workspace_t* workspace, int tree_index, const char* scope_name, int selector_index)
{
  if(workspace == NULL) {
    fprintf(stderr, "html_select() - Empty workspace.\n");
    return -1;
  }

  myhtml_tree_node_t* scope_node = html_get_scope_node(workspace, tree_index, scope_name);
  if(scope_node == NULL) {
    return -1;
  }

#ifdef MODEST_HTML_DEBUG
  // printf("html_select()\n");
  // printf("\tscope_name = %s\n", scope_name);
  // if(myhtml_node_next(scope_node)) {
  //   printf("\texecute selector on each node in the scope...\n");
  // }
  printf("html_select()\n\t");
#endif

  myhtml_collection_t* collection = NULL;
  myhtml_tree_node_t* node = scope_node;
  while(node) {
    const char* tag_name = myhtml_tag_name_by_id(node->tree, myhtml_node_tag_id(node), NULL);
    /*if(strcmp(tag_name, "-text") == 0) {
      if(collection == NULL) {
        mystatus_t status;
        collection = myhtml_collection_create(1, &status);
      }
      if(myhtml_collection_check_size(collection, 1, 1024) == MyHTML_STATUS_OK) {
        collection->list[collection->length] = node;
        collection->length++;
    #ifdef MODEST_HTML_DEBUG
        printf("\t%d: %s\n", (int)collection->length, tag_name);
    #endif
      }
      else {
        fprintf(stderr, "html_select() - Failed to append text node to collection.");
      }
    }
    else*/ {
      modest_finder_t* finder = workspace->finder;
      mycss_selectors_list_t* list = workspace->selectors.data[selector_index];
      modest_finder_by_selectors_list(finder, node, list, &collection);
#ifdef MODEST_HTML_DEBUG
      // printf("\ttag '%s': list size = %d\n", tag_name, (int)collection->length);
      printf("%s ", tag_name);
#endif
    }
    node = myhtml_node_next(node);
  }

#ifdef MODEST_HTML_DEBUG
  // printf("\n\tcollection has %d nodes\n", (int)collection->length);
  printf("\n");
#endif

  if(collection && collection->list && collection->length) {
    html_vec_push(&workspace->collections, collection);
    return workspace->collections.length - 1;
  }

  return -1;
}

int html_select_scope(html_workspace_t* workspace, int tree_index, const char* scope_name)
{
  if(workspace == NULL) {
    fprintf(stderr, "html_select_scope() - Empty workspace.\n");
    return -1;
  }

  myhtml_tree_node_t* scope_node = html_get_scope_node(workspace, tree_index, scope_name);
  if(scope_node == NULL) {
    return -1;
  }

#ifdef MODEST_HTML_DEBUG
  // printf("html_select_scope()\n");
  // printf("\tscope_name = %s\n", scope_name);
  // if(myhtml_node_next(scope_node)) {
  //   printf("\tcollect each node in the scope...\n");
  // }
  printf("html_select_scope()\n\t");
#endif

  myhtml_collection_t* collection = NULL;
  myhtml_tree_node_t* node = scope_node;
  while(node) {
    if(collection == NULL) {
      mystatus_t status;
      collection = myhtml_collection_create(1, &status);
    }
    if(myhtml_collection_check_size(collection, 1, 1024) == MyHTML_STATUS_OK) {
      collection->list[collection->length] = node;
      collection->length++;
#ifdef MODEST_HTML_DEBUG
      const char* tag_name = myhtml_tag_name_by_id(node->tree, myhtml_node_tag_id(node), NULL);
      // printf("\ttag '%s': list size = %d\n", tag_name, (int)collection->length);
      printf("%s ", tag_name);
#endif
    }
    // try text
    node = myhtml_node_next(node);
  }

#ifdef MODEST_HTML_DEBUG
  // printf("\n\tcollection has %d nodes\n", (int)collection->length);
  printf("\n");
#endif

  if(collection && collection->list && collection->length) {
    html_vec_push(&workspace->collections, collection);
    return workspace->collections.length - 1;
  }

  return -1;
}