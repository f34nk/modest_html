
#include "modest_html.h"

int main(int argc, char const *argv[])
{
  html_workspace_t *w = html_init();
  int i = 0;

  // first tree
  
  const char *html = "<html><head></head><body><p>Hello World</p></body></html>";
  const char *selector = "p";
  html_result_t s1 = html_parse_and_select(w, html, selector);

  const char *new_html = "<span>Append Me</span>";
  const char *new_selector = "body *";
  html_result_t new_s1 = html_parse_and_select(w, new_html, new_selector);

  int buffer_index;
  vec_str_t *buffer;
  char *result;
  const char *scope_name = "body";

  html_append_collection(w, s1.collection_index, new_s1.collection_index);
  buffer_index = html_serialize_collection(w, s1.collection_index);
  buffer = html_get_buffer(w, buffer_index);
  result = html_vec_str_join(buffer, "|");
  printf("%d: %s\n", ++i, result);
  if(result != NULL && strcmp(result, "<p>Hello World<span>Append Me</span></p>") != 0){
    fprintf(stderr, "Failed\n");
    html_free(result);
    html_destroy(w);
    return 1;
  }
  html_free(result);

  // second tree
  
  // TODO: support appending to multiple nodes 
  
  html = "<p>Hello</p><p>World</p>";
  selector = "p";
  html_result_t s2 = html_parse_and_select(w, html, selector);

  new_html = "<span>Append Me</span>";
  new_selector = "body *";
  html_result_t new_s2 = html_parse_and_select(w, new_html, new_selector);

  // Not supported until now.
  html_append_collection(w, s2.collection_index, new_s2.collection_index);

  // scope_name = "body";
  // buffer_index = html_serialize_tree(w, new_s2.tree_index, scope_name);
  // buffer_index = html_serialize_collection(w, s2.collection_index);
  // buffer = html_get_buffer(w, buffer_index);
  // result = html_vec_str_join(buffer, "|");
  // printf("%d: %s\n", ++i, result);
  // if(result != NULL && strcmp(result, "asdasd") != 0){
  //   fprintf(stderr, "Failed\n");
  //   html_free(result);
  //   html_destroy(w);
  //   return 1;
  // }
  // html_free(result);

  // third tree
  
  html = "<p>Hello World</p>";
  selector = "p";
  html_result_t s3 = html_parse_and_select(w, html, selector);

  new_html = "<span>Append</span><span>Me</span>";
  new_selector = "body *";
  html_result_t new_s3 = html_parse_and_select(w, new_html, new_selector);

  html_append_collection(w, s3.collection_index, new_s3.collection_index);
  scope_name = "body";
  buffer_index = html_serialize_tree(w, s3.tree_index, scope_name);
  buffer = html_get_buffer(w, buffer_index);
  result = html_vec_str_join(buffer, "|");
  printf("%d: %s\n", ++i, result);
  if(result != NULL && strcmp(result, "<body><p>Hello World<span>Append</span><span>Me</span></p></body>") != 0){
    fprintf(stderr, "Failed\n");
    html_free(result);
    html_destroy(w);
    return 1;
  }
  html_free(result);

  buffer_index = html_serialize_collection(w, s3.collection_index);
  buffer = html_get_buffer(w, buffer_index);
  result = html_vec_str_join(buffer, "|");
  printf("%d: %s\n", ++i, result);
  if(result != NULL && strcmp(result, "<p>Hello World<span>Append</span><span>Me</span></p>") != 0){
    fprintf(stderr, "Failed\n");
    html_free(result);
    html_destroy(w);
    return 1;
  }
  html_free(result);


  // TODO: check select string only; implement append_buffer()
  

  html_destroy(w);
  printf("ok\n");
  return 0;
}