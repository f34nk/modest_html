
#include "modest_html.h"

int main(int argc, char const *argv[])
{
  html_workspace_t *w = html_init();
  int i = 0;
  
  const char *html = "<p>Hello</p><p>World</p>";
  const char *selector = "p";
  
  int tree_index = html_parse_tree(w, html, strlen(html));
  int selector_index = html_prepare_selector(w, selector, strlen(selector));
  const char *scope_name = "body_children";
  int collection_index  = html_select(w, tree_index, scope_name, selector_index);

  int buffer_index = html_position(w, collection_index);
  html_vec_str_t *buffer = html_get_buffer(w, buffer_index);
  char *result = html_vec_join(buffer, "|");
  printf("-> %s\n", result);
  if(strcmp(result, "1|2") != 0){
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