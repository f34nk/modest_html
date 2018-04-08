
#include "modest_html.h"

int wrap_in_node_with_text_test(html_workspace_t *w)
{
  MODEST_HTML_LOG

  const char *html = "<html><head></head><body><div><p class=\"hello\">Hello</p>World</div></body></html>";
  const char *selector = "p";
  
  int tree_index = html_parse_tree(w, html, strlen(html));
  int selector_index = html_prepare_selector(w, selector, strlen(selector));
  const char *scope_name = "html";
  int collection_index  = html_select(w, tree_index, scope_name, selector_index);

  const char *new_html = "<span>Hello</span>";
  const char *new_selector = "body *";
  int new_tree_index = html_parse_tree(w, new_html, strlen(new_html));
  int new_selector_index = html_prepare_selector(w, new_selector, strlen(new_selector));
  const char *new_scope_name = "html";
  int new_collection_index  = html_select(w, new_tree_index, new_scope_name, new_selector_index);

  html_wrap(w, collection_index, new_collection_index);

  int buffer_index = html_serialize_tree(w, tree_index, "body");
  html_vec_str_t *buffer = html_get_buffer(w, buffer_index);
  char *result = html_vec_join(buffer, "|");
  printf("-> %s\n", result);
  if(strcmp(result, "<body><div><span>Hello<p class=\"hello\">Hello</p></span>World</div></body>") != 0){
    fprintf(stderr, "Failed\n");
    html_free(result);
    MODEST_HTML_ERROR
    return 1;
  }
  html_free(result);

  return 0;
}

int wrap_with_text_only_test(html_workspace_t *w)
{
  MODEST_HTML_LOG

  // TODO: implement
  return 0;
}

#define max_tests 2
int (*test[max_tests])() = {wrap_in_node_with_text_test, wrap_with_text_only_test};

int main(int argc, char const *argv[])
{
  html_workspace_t *w = html_init();

  int i = 0;
  int result = 0;
  while(i < max_tests && result == 0){
    result = test[i](w);
    i += 1;
  }

  html_destroy(w);  
  printf("ok\n");
  return 0;
}