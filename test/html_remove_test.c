
#include "modest_html.h"

int main(int argc, char const *argv[])
{
  html_workspace_t *w = html_init();
  int i = 0;

  // first tree
  
  const char *html = "<html><head></head><body><div><p class=\"hello\">Hello</p>World</div></body></html>";
  const char *selector = "p";
  html_result_t s1 = html_parse_and_select(w, html, selector);

  html_remove(w, s1.collection_index);
  const char *scope_name = "body";
  int buffer_index = html_serialize_tree(w, s1.tree_index, scope_name);
  html_vec_t *buffer = html_get_buffer(w, buffer_index);
  char *result = html_vec_join(buffer, "|");
  printf("%d: %s\n", ++i, result);
  if(result != NULL && strcmp(result, "<body><div>World</div></body>") != 0){
    fprintf(stderr, "Failed\n");
    html_free(result);
    html_destroy(w);
    return 1;
  }
  html_free(result);

  // second tree
  
  html = "<p>Lorem</p><p>ipsum<a href=\"http://google.de\">dolor sit</a></p>";
  selector = "a";
  html_result_t s2 = html_parse_and_select(w, html, selector);

  html_remove(w, s2.collection_index);
  scope_name = "body";
  buffer_index = html_serialize_tree(w, s2.tree_index, scope_name);
  buffer = html_get_buffer(w, buffer_index);
  result = html_vec_join(buffer, "|");
  printf("%d: %s\n", ++i, result);
  if(result != NULL && strcmp(result, "<body><p>Lorem</p><p>ipsum</p></body>") != 0){
    fprintf(stderr, "Failed\n");
    html_free(result);
    html_destroy(w);
    return 1;
  }
  html_free(result);

  html_destroy(w);
  printf("ok\n");
  return 0;
}