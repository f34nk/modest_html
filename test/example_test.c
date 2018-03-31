#include "modest_html.h"
/*
To check for memory leaks execute test with valgrind.

  valgrind --leak-check=yes test/vec_join_test

*/
int main(int argc, char const *argv[])
{
  html_workspace_t *w = html_init();
  int i = 0;

  const char *html = "<html><head></head><body><div><p class=\"hello\">Hello</p>World</div></body></html>";
  const char *selector = "p";
  html_result_t s1 = html_parse_and_select(w, html, selector);

  const char *scope_name = "html";
  int buffer_index = html_serialize_tree(w, s1.tree_index, scope_name);
  vec_str_t *buffer = html_get_buffer(w, buffer_index);
  char* result = html_vec_str_join(buffer, "|");
  printf("%d: %s\n", ++i, result);
  if(strcmp(result, "<html><head></head><body><div><p class=\"hello\">Hello</p>World</div></body></html>") != 0){
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