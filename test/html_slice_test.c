
#include "modest_html.h"

int main(int argc, char const *argv[])
{
  html_workspace_t *w = html_init();
  int i = 0;

  // first tree
  
  const char *html = "<html><head></head><body><a href=\"http://google.de\">Link</a><p>Hello</p><div>World</div></body></html>";
  const char *selector = "body *";
  html_result_t s1 = html_parse_and_select(w, html, selector);

  int collection_index = html_slice(w, s1.collection_index, 1, -1);
  int buffer_index = html_serialize_collection(w, collection_index);
  vec_str_t *buffer = html_get_buffer(w, buffer_index);
  char *result = html_vec_str_join(buffer, "|");
  printf("%d: %s\n", ++i, result);
  if(strcmp(result, "<p>Hello</p>|<div>World</div>") != 0){
    fprintf(stderr, "Failed\n");
    html_free(result);
    html_destroy(w);
    return 1;
  }
  html_free(result);

  // second tree
  
  html = "<div><p>Lorem</p><p>ipsum<a href=\"http://google.de\">dolor sit</a></p></div>";
  selector = "div *";
  html_result_t s2 = html_parse_and_select(w, html, selector);

  collection_index = html_slice(w, s2.collection_index, 0, 1);
  buffer_index = html_serialize_collection(w, collection_index);
  buffer = html_get_buffer(w, buffer_index);
  result = html_vec_str_join(buffer, "|");
  printf("%d: %s\n", ++i, result);
  if(strcmp(result, "<p>Lorem</p>") != 0){
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