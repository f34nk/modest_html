#include "html_parse_and_select.h"
#include "html_parse.h"
#include "html_select.h"

html_result_t html_parse_and_select(html_workspace_t *workspace, const char *html, const char *selector)
{
  html_workspace_t *w = workspace;
  html_result_t result;
  result.tree_index = -1;
  result.selector_index = -1;
  result.collection_index = -1;

  if(w == NULL) {
    fprintf(stderr, "html_parse_and_select() - Empty workspace.\n");
    return result;
  }

  int tree_index = html_parse_tree(w, html, strlen(html));
#ifdef MODEST_HTML_DEBUG
  if(tree_index == -1) {
    printf("html_parse_and_select() - Failed to parse data.\n");
  }
  // else {
  //   printf("html_parse_and_select() - Parsed data.\n");
  // }
#endif

  int selector_index = html_prepare_selector(w, selector, strlen(selector));
#ifdef MODEST_HTML_DEBUG
  if(selector_index == -1) {
    printf("html_parse_and_select() - Failed to prepare selector.\n");
  }
  // else {
  //   printf("html_parse_and_select() - Prepared selector.\n");
  // }
#endif

  int collection_index  = html_select(w, tree_index, "html", selector_index);
#ifdef MODEST_HTML_DEBUG
  if(collection_index == -1) {
    printf("html_parse_and_select() - Failed to find collection.\n");
  }
  // else {
  //   printf("html_parse_and_select() - Found collection.\n");
  // }
#endif

  result.tree_index = tree_index;
  result.selector_index = selector_index;
  result.collection_index = collection_index;

#ifdef MODEST_HTML_DEBUG
  printf("html_parse_and_select() - ok\n");
#endif

  return result;
}