
#include "modest_html.h"

int main(int argc, char const *argv[])
{
  html_workspace_t *w = html_init();
  int i = 0;
  
  const char *html = "<div><p>Hello</p><p>World</p></div>";
  const char *selector = "p";
  html_result_t s1 = html_parse_and_select(w, html, selector);

  html = "<div><p>Hello</p><span>world</span></div>";
  selector = "p";
  html_result_t s2 = html_parse_and_select(w, html, selector);

  int buffer_index = html_compare(w, s1.collection_index, s2.collection_index);
  vec_str_t *buffer = html_get_buffer(w, buffer_index);
  char *result = html_vec_str_join(buffer, "|");
  printf("%d: %s\n", ++i, result);
  if(strcmp(result, "asdasdasd") != 0){
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