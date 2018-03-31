#include "html_select.h"

mystatus_t html_serialization_bad_selectors(const char *data, size_t len, void *ctx)
{
  fprintf(stderr, "%.*s", (int)len, data);
  return MyCORE_STATUS_OK;
}

int html_prepare_selector(html_workspace_t *workspace, const char *selector, size_t selector_size)
{
  if(workspace == NULL) {
    fprintf(stderr, "html_prepare_selector() - Empty workspace.\n");
    return -1;
  }

  mycss_t *mycss = workspace->mycss;

  // init current entry
  mycss_entry_t *entry = mycss_entry_create();
  mystatus_t status = mycss_entry_init(mycss, entry);
  if(status != MyCORE_STATUS_OK) {
    fprintf(stderr, "html_prepare_selector() - Failed to init css entry\n");
    return -1;
  }

  mycss_selectors_list_t *list = mycss_selectors_parse(mycss_entry_selectors(entry), MyENCODING_UTF_8, selector, selector_size, &status);

  // check parsing errors
  if(list == NULL || (list->flags & MyCSS_SELECTORS_FLAGS_SELECTOR_BAD)) {
    fprintf(stderr, "html_prepare_selector() - Bad CSS Selectors\n");
    if(list) {
      mycss_selectors_serialization_list(mycss_entry_selectors(entry), list, html_serialization_bad_selectors, NULL);
      fprintf(stderr, "\n");
    }
    return -1;
  }

  vec_push(&workspace->entries, entry);
  vec_push(&workspace->selectors, list);

  return workspace->selectors.length - 1;
}

int html_select(html_workspace_t *workspace, int tree_index, const char *scope_name, int selector_index)
{
  if(workspace == NULL) {
    fprintf(stderr, "html_select() - Empty workspace.\n");
    return -1;
  }

  myhtml_collection_t *collection = NULL;
  modest_finder_t *finder = workspace->finder;
  myhtml_tree_node_t *scope_node = html_get_scope_node(workspace, tree_index, scope_name);
  if(scope_node == NULL) {
    fprintf(stderr, "html_select() - No node for scope '%s'.\n", scope_name);
    return -1;
  }

  mycss_selectors_list_t *list = workspace->selectors.data[selector_index];
  modest_finder_by_selectors_list(finder, scope_node, list, &collection);
  if(collection && collection->list && collection->length) {
    vec_push(&workspace->collections, collection);
    return workspace->collections.length - 1;
  }
  
  return -1;
}