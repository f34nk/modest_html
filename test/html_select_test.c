
#include "modest_html.h"

int main(int argc, char const *argv[])
{
  html_workspace_t *w = html_init();
  int i = 0;

  // test

  const char *html = "<html><head></head><body><div><p class=\"hello\">Hello</p>World</div></body></html>";
  const char *selector = "p";

  int tree_index = html_parse_tree(w, html, strlen(html));
  int selector_index = html_prepare_selector(w, selector, strlen(selector));

  const char *scope_name = "html";
  int collection_index  = html_select(w, tree_index, scope_name, selector_index);

  int buffer_index = html_serialize_collection(w, collection_index);
  html_vec_str_t *buffer = html_get_buffer(w, buffer_index);
  char* result = html_vec_join(buffer, "|");
  printf("%d: %s\n", ++i, result);
  if(strcmp(result, "<p class=\"hello\">Hello</p>") != 0){
    fprintf(stderr, "Failed\n");
    html_free(result);
    html_destroy(w);
    return 1;
  }
  html_free(result);

  // test

  html = "Lorem<p>ipsum</p><span>dolor</span>";
  selector = "body > *";

  tree_index = html_parse_tree(w, html, strlen(html));
  selector_index = html_prepare_selector(w, selector, strlen(selector));

  scope_name = "body";
  collection_index  = html_select(w, tree_index, scope_name, selector_index);

  buffer_index = html_serialize_collection(w, collection_index);
  buffer = html_get_buffer(w, buffer_index);
  result = html_vec_join(buffer, "|");
  printf("%d: %s\n", ++i, result);
  if(strcmp(result, "<p>ipsum</p>|<span>dolor</span>") != 0){
    fprintf(stderr, "Failed\n");
    html_free(result);
    html_destroy(w);
    return 1;
  }
  html_free(result);

  // same as this test

  html = "Lorem<p>ipsum</p><span>dolor</span>";
  selector = "*";

  tree_index = html_parse_tree(w, html, strlen(html));
  selector_index = html_prepare_selector(w, selector, strlen(selector));

  scope_name = "body_children";
  collection_index  = html_select(w, tree_index, scope_name, selector_index);

  buffer_index = html_serialize_collection(w, collection_index);
  buffer = html_get_buffer(w, buffer_index);
  result = html_vec_join(buffer, "|");
  printf("%d: %s\n", ++i, result);
  if(strcmp(result, "<p>ipsum</p>|<span>dolor</span>") != 0){
    fprintf(stderr, "Failed\n");
    html_free(result);
    html_destroy(w);
    return 1;
  }
  html_free(result);

  // test

  html = "<p>Hello</p><p>World</p>Test";
  selector = "p";

  tree_index = html_parse_tree(w, html, strlen(html));
  selector_index = html_prepare_selector(w, selector, strlen(selector));

  scope_name = "body_children";
  collection_index  = html_select(w, tree_index, scope_name, selector_index);

  buffer_index = html_serialize_collection(w, collection_index);
  buffer = html_get_buffer(w, buffer_index);
  result = html_vec_join(buffer, "|");
  printf("%d: %s\n", ++i, result);
  if(strcmp(result, "<p>Hello</p>|<p>World</p>") != 0){
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