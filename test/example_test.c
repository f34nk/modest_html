#include "modest_html.h"
/*
To check for memory leaks execute test with valgrind.

  valgrind --leak-check=yes test/vec_join_test

*/
int main(int argc, char const* argv[])
{
  html_workspace_t* w = html_init();
  int i = 0;

  const char* html = "<html><head></head><body><div><p class=\"hello\">Hello</p>World</div></body></html>";

  int tree_index = html_parse_tree(w, html, strlen(html));

  const char* scope_name = "html";
  int buffer_index = html_serialize_tree(w, tree_index, scope_name);
  html_vec_str_t* buffer = html_get_buffer(w, buffer_index);
  char* result = html_vec_join(buffer, "|");
  printf("-> %s\n", result);
  if(strcmp(result, "<html><head></head><body><div><p class=\"hello\">Hello</p>World</div></body></html>") != 0) {
    fprintf(stderr, "Failed\n");
    html_free(result);
    html_destroy(w);
    return 1;
  }
  html_free(result);

  html_destroy(w);
  printf("done\n");
  return 0;
}