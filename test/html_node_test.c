#include "modest_html.h"

int main(int argc, char const* argv[])
{
  html_workspace_t* w = html_init();
  int i = 0;

  const char* html = "<p class=\"hello\" id=\"world\">Hello</p>";
  const char* selector = "p";

  int tree_index = html_parse_tree(w, html, strlen(html));
  int selector_index = html_prepare_selector(w, selector, strlen(selector));
  const char* scope_name = "body_children";
  int collection_index  = html_select(w, tree_index, scope_name, selector_index);

  myhtml_tree_node_t* node = html_get_node(w, collection_index, 0);

  html_node_t params;
  html_node_init(&params);
  html_node_get(node, &params);
  html_node_dump(stdout, &params);
  html_node_destroy(&params);

  html_destroy(w);
  printf("done\n");
  return 0;
}