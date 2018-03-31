
#include "modest_html.h"

int main(int argc, char const *argv[])
{
  html_workspace_t *w = html_init();
  int i = 0;

  // first tree
  
  const char *html = "<html><head></head><body><div><p class=\"hello\">Hello</p>World</div></body></html>";
  const char *selector = "p";
  html_result_t s1 = html_parse_and_select(w, html, selector);

  int attributes_index = html_get_attribute(w, s1.collection_index, "class");
  vec_str_t *attributes = html_get_buffer(w, attributes_index);
  char *result = html_vec_str_join(attributes, "|");
  printf("%d: %s\n", ++i, result);
  if(strcmp(result, "hello") != 0){
    fprintf(stderr, "Failed\n");
    html_free(result);
    html_destroy(w);
    return 1;
  }
  html_free(result);

  html_set_attribute(w, s1.collection_index, "class", "changed");
  int buffer_index = html_serialize_collection(w, s1.collection_index);
  vec_str_t *buffer = html_get_buffer(w, buffer_index);
  result = html_vec_str_join(buffer, "|");
  printf("%d: %s\n", ++i, result);
  if(strcmp(result, "<p class=\"changed\">Hello</p>") != 0){
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

  attributes_index = html_get_attribute(w, s2.collection_index, "href");
  attributes = html_get_buffer(w, attributes_index);
  result = html_vec_str_join(attributes, "|");
  printf("%d: %s\n", ++i, result);
  if(strcmp(result, "http://google.de") != 0){
    fprintf(stderr, "Failed\n");
    html_free(result);
    html_destroy(w);
    return 1;
  }
  html_free(result);

  html_set_attribute(w, s2.collection_index, "class", "changed");
  buffer_index = html_serialize_collection(w, s2.collection_index);
  buffer = html_get_buffer(w, buffer_index);
  result = html_vec_str_join(buffer, "|");
  printf("%d: %s\n", ++i, result);
  if(strcmp(result, "<a href=\"http://google.de\" class=\"changed\">dolor sit</a>") != 0){
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