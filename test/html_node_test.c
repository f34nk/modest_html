#include "modest_html.h"

int main(int argc, char const *argv[])
{
  html_workspace_t *w = html_init();
  int i = 0;

  const char *html = "<p class=\"hello\" id=\"world\">Hello</p>";
  const char *selector = "p";
  html_result_t s1 = html_parse_and_select(w, html, selector);

  myhtml_tree_node_t *node = html_get_node(w, s1.collection_index, 0);

  html_node_t params;
  html_node_init(&params);
  html_node_get(node, &params);
  html_node_dump(stdout, &params);
  html_node_destroy(&params);

  html_destroy(w);
  printf("ok\n");
  return 0;
}