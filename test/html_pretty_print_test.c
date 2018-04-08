
#include "modest_html.h"

int main(int argc, char const *argv[])
{
  html_workspace_t *w = html_init();
  int i = 0;

  // test

  const char *html = "<div class=\"block\"><p>Hello</p><p>World</p><img src=\"test.png\"/></div>";
  const char *selector = "p";
  int tree_index = html_parse_tree(w, html, strlen(html));
  int selector_index = html_prepare_selector(w, selector, strlen(selector));
  const char *scope_name = "body_children";
  int collection_index  = html_select(w, tree_index, scope_name, selector_index);

  bool colorize = false;
  char *result = html_pretty_print(w, collection_index, colorize);

  printf("%d: %s", ++i, result);
  if(strcmp(result, "<p>\n\tHello\n</p>\n<p>\n\tWorld\n</p>\n") != 0){
    fprintf(stderr, "Failed\n");
    html_free(result);
    html_destroy(w);
    MODEST_HTML_ERROR
    return 1;
  }
  html_free(result);

  html_destroy(w);
  printf("ok\n");
  return 0;
}