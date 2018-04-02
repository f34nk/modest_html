
#include "modest_html.h"

int main(int argc, char const *argv[])
{
  html_workspace_t *w = html_init();
  int i = 0;

  // first tree
  
  const char *html = "<html><head></head><body><div><p class=\"hello\">Hello</p>World</div></body></html>";
  const char *selector = "p";
  html_result_t s1 = html_parse_and_select(w, html, selector);

  int text_index = html_get_text(w, s1.collection_index);
  html_vec_t *text = html_get_buffer(w, text_index);
  char *result = html_vec_join(text, "|");
  printf("%d: %s\n", ++i, result);
  if(strcmp(result, "Hello") != 0){
    fprintf(stderr, "Failed\n");
    html_free(result);
    html_destroy(w);
    return 1;
  }
  html_free(result);

  html_set_text(w, s1.collection_index, "Changed");
  int buffer_index = html_serialize_collection(w, s1.collection_index);
  html_vec_t *buffer = html_get_buffer(w, buffer_index);
  result = html_vec_join(buffer, "|");
  printf("%d: %s\n", ++i, result);
  if(strcmp(result, "<p class=\"hello\">Changed</p>") != 0){
    fprintf(stderr, "Failed\n");
    html_free(result);
    html_destroy(w);
    return 1;
  }
  html_free(result);

  // second tree
  
  html = "<p>Lorem</p><p>ipsum<a href=\"http://google.de\">dolor sit</a></p>";
  selector = "p,a";
  html_result_t s2 = html_parse_and_select(w, html, selector);

  text_index = html_get_text(w, s2.collection_index);
  text = html_get_buffer(w, text_index);
  result = html_vec_join(text, "|");
  printf("%d: %s\n", ++i, result);
  if(strcmp(result, "Lorem|ipsum|dolor sit") != 0){
    fprintf(stderr, "Failed\n");
    html_free(result);
    html_destroy(w);
    return 1;
  }
  html_free(result);

  // TODO: inplace set_text
  
  html_set_text(w, s2.collection_index, "hello");
  buffer_index = html_serialize_collection(w, s2.collection_index);
  buffer = html_get_buffer(w, buffer_index);
  result = html_vec_join(buffer, "|");
  printf("%d: %s\n", ++i, result);
  // if(strcmp(result, "<p>hello</p>|<p>hello<a href=\"http://google.de\">hello</a></p>|<a href=\"http://google.de\">hello</a>") != 0){
  if(strcmp(result, "<p>hello</p>|<p><a href=\"http://google.de\">hello</a>hello</p>|<a href=\"http://google.de\">hello</a>") != 0){
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