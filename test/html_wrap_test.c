
#include "modest_html.h"

int main(int argc, char const *argv[])
{
  html_workspace_t *w = html_init();
  int i = 0;

  // first tree
  
  const char *html = "<html><head></head><body><div><p class=\"hello\">Hello</p>World</div></body></html>";
  const char *selector = "p";
  html_result_t s1 = html_parse_and_select(w, html, selector);

  const char *new_html = "<span>Hello</span>";
  const char *new_selector = "body *";
  html_result_t new_s1 = html_parse_and_select(w, new_html, new_selector);

  html_wrap(w, s1.collection_index, new_s1.collection_index);
  const char *scope_name = "body";
  int buffer_index = html_serialize_tree(w, s1.tree_index, scope_name);
  html_vec_t *buffer = html_get_buffer(w, buffer_index);
  char *result = html_vec_join(buffer, "|");
  printf("%d: %s\n", ++i, result);
  if(result != NULL && strcmp(result, "<body><div><span>Hello<p class=\"hello\">Hello</p></span>World</div></body>") != 0){
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

  new_html = "Hello World";
  new_selector = "body *";
  html_result_t new_s2 = html_parse_and_select(w, new_html, new_selector);

  // TODO: check select string only
  
  // html_wrap(w, s2.collection_index, new_s2.collection_index);
  // scope_name = "body";
  // buffer_index = html_serialize_tree(w, s2.tree_index, scope_name);
  // buffer = html_get_buffer(w, buffer_index);
  // result = html_vec_join(buffer, "|");
  // printf("%d: %s\n", ++i, result);
  // if(result != NULL && strcmp(result, "sdasd") != 0){
  //   fprintf(stderr, "Failed\n");
  //   html_free(result);
  //   html_destroy(w);
  //   return 1;
  // }
  // html_free(result);

  html_destroy(w);
  printf("ok\n");
  return 0;
}