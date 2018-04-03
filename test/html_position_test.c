
#include "modest_html.h"

int main(int argc, char const *argv[])
{
  html_workspace_t *w = html_init();
  int i = 0;

  // first tree
  
  const char *html = "<p>Hello</p><p>World</p>";
  const char *selector = "p";
  html_result_t s1 = html_parse_and_select(w, html, selector);

  int buffer_index = html_position(w, s1.collection_index);
  html_vec_str_t *buffer = html_get_buffer(w, buffer_index);
  char *result = html_vec_join(buffer, "|");
  printf("%d: %s\n", ++i, result);
  if(strcmp(result, "1|2") != 0){
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