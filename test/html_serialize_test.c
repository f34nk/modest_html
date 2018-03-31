
#include "modest_html.h"

int main(int argc, char const *argv[])
{
  html_workspace_t *w = html_init();
  int i = 0;

  // first tree
  
  const char *html = "<html><head></head><body><div><p class=\"hello\">Hello</p>World</div></body></html>";
  const char *selector = "p";
  html_result_t s1 = html_parse_and_select(w, html, selector);

  const char *scope_name = "html";
  int buffer_index = html_serialize_tree(w, s1.tree_index, scope_name);
  vec_str_t *buffer = html_get_buffer(w, buffer_index);
  char* result = html_vec_str_join(buffer, "|");
  printf("%d: %s\n", ++i, result);
  if(result != NULL && strcmp(result, "<html><head></head><body><div><p class=\"hello\">Hello</p>World</div></body></html>") != 0){
    fprintf(stderr, "Failed\n");
    html_free(result);
    html_destroy(w);
    return 1;
  }
  html_free(result);

  buffer_index = html_serialize_collection(w, s1.collection_index);
  buffer = html_get_buffer(w, buffer_index);
  result = html_vec_str_join(buffer, "|");
  printf("%d: %s\n", ++i, result);
  if(result != NULL && strcmp(result, "<p class=\"hello\">Hello</p>") != 0){
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

  scope_name = "body";
  buffer_index = html_serialize_tree(w, s2.tree_index, scope_name);
  buffer = html_get_buffer(w, buffer_index);
  result = html_vec_str_join(buffer, "|");
  printf("%d: %s\n", ++i, result);
  if(result != NULL && strcmp(result, "<body><p>Lorem</p><p>ipsum<a href=\"http://google.de\">dolor sit</a></p></body>") != 0){
    fprintf(stderr, "Failed\n");
    html_free(result);
    html_destroy(w);
    return 1;
  }
  html_free(result);

  buffer_index = html_serialize_collection(w, s2.collection_index);
  buffer = html_get_buffer(w, buffer_index);
  result = html_vec_str_join(buffer, "|");
  printf("%d: %s\n", ++i, result);
  if(result != NULL && strcmp(result, "<p>Lorem</p>|<p>ipsum<a href=\"http://google.de\">dolor sit</a></p>|<a href=\"http://google.de\">dolor sit</a>") != 0){
    fprintf(stderr, "Failed\n");
    html_free(result);
    html_destroy(w);
    return 1;
  }
  html_free(result);

  // third tree

  html = "Hello World";
  selector = "body *";
  html_result_t s3 = html_parse_and_select(w, html, selector);

  // TODO: check select string only

  // buffer_index = html_serialize_collection(w, s3.collection_index);
  // buffer = html_get_buffer(w, buffer_index);
  // result = html_vec_str_join(buffer, "|");
  // printf("%d: %s\n", ++i, result);
  // if(result != NULL && strcmp(result, "") != 0){
  //   fprintf(stderr, "Failed\n");
  //   html_free(result);
  //   html_destroy(w);
  //   return 1;
  // }
  // html_free(result);

  scope_name = "body";
  buffer_index = html_serialize_tree(w, s3.tree_index, scope_name);
  buffer = html_get_buffer(w, buffer_index);
  result = html_vec_str_join(buffer, "|");
  printf("%d: %s\n", ++i, result);
  if(result != NULL && strcmp(result, "<body>Hello World</body>") != 0){
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