
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
  html_vec_str_t *buffer = html_get_buffer(w, buffer_index);
  char* result = html_vec_join(buffer, "|");
  printf("%d: %s\n", ++i, result);
  if(strcmp(result, "<html><head></head><body><div><p class=\"hello\">Hello</p>World</div></body></html>") != 0){
    fprintf(stderr, "Failed\n");
    html_free(result);
    html_destroy(w);
    return 1;
  }
  html_free(result);

  buffer_index = html_serialize_collection(w, s1.collection_index);
  buffer = html_get_buffer(w, buffer_index);
  result = html_vec_join(buffer, "|");
  printf("%d: %s\n", ++i, result);
  if(strcmp(result, "<p class=\"hello\">Hello</p>") != 0){
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
  result = html_vec_join(buffer, "|");
  printf("%d: %s\n", ++i, result);
  if(strcmp(result, "<body><p>Lorem</p><p>ipsum<a href=\"http://google.de\">dolor sit</a></p></body>") != 0){
    fprintf(stderr, "Failed\n");
    html_free(result);
    html_destroy(w);
    return 1;
  }
  html_free(result);

  buffer_index = html_serialize_collection(w, s2.collection_index);
  buffer = html_get_buffer(w, buffer_index);
  result = html_vec_join(buffer, "|");
  printf("%d: %s\n", ++i, result);
  if(strcmp(result, "<p>Lorem</p>|<p>ipsum<a href=\"http://google.de\">dolor sit</a></p>|<a href=\"http://google.de\">dolor sit</a>") != 0){
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
  // result = html_vec_join(buffer, "|");
  // printf("%d: %s\n", ++i, result);
  // if(strcmp(result, "") != 0){
  //   fprintf(stderr, "Failed\n");
  //   html_free(result);
  //   html_destroy(w);
  //   return 1;
  // }
  // html_free(result);

  scope_name = "body_children";
  buffer_index = html_serialize_tree(w, s3.tree_index, scope_name);
  buffer = html_get_buffer(w, buffer_index);
  result = html_vec_join(buffer, "|");
  printf("%d: %s\n", ++i, result);
  if(strcmp(result, "Hello World") != 0){
    fprintf(stderr, "Failed\n");
    html_free(result);
    html_destroy(w);
    return 1;
  }
  html_free(result);

  // fourth tree
  html = "<div>Hello</div><div><span><p><a>World</a></p></span></div>";
  selector = "a";
  html_result_t s4 = html_parse_and_select(w, html, selector);

  myhtml_collection_t *collection = (myhtml_collection_t*)html_get_collection(w, s4. collection_index);
  myhtml_tree_node_t *node = collection->list[0];
  result = html_serialize_node(node);
  printf("%d: %s\n", ++i, result);
  if(strcmp(result, "<a>World</a>") != 0){
    fprintf(stderr, "Failed\n");
    html_free(result);
    html_destroy(w);
    return 1;
  }
  html_free(result);

  result = html_serialize_selector(node);
  printf("%d: %s\n", ++i, result);
  if(strcmp(result, "html body div span p a") != 0){
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