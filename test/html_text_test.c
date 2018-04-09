
#include "modest_html.h"

int get_and_set_text_test(html_workspace_t* w)
{
  MODEST_HTML_LOG

  const char* html = "<html><head></head><body><div><p class=\"hello\">Hello</p>World</div></body></html>";
  const char* selector = "p";

  int tree_index = html_parse_tree(w, html, strlen(html));
  int selector_index = html_prepare_selector(w, selector, strlen(selector));
  const char* scope_name = "html";
  int collection_index  = html_select(w, tree_index, scope_name, selector_index);

  int text_index = html_get_text(w, collection_index);

  html_vec_str_t* text = html_get_buffer(w, text_index);
  char* result = html_vec_join(text, "|");
  printf("-> %s\n", result);
  if(strcmp(result, "Hello") != 0) {
    fprintf(stderr, "Failed\n");
    html_free(result);
    MODEST_HTML_ERROR
    return 1;
  }
  html_free(result);

  html_set_text(w, collection_index, "Changed");

  int buffer_index = html_serialize_collection(w, collection_index);
  html_vec_str_t* buffer = html_get_buffer(w, buffer_index);
  result = html_vec_join(buffer, "|");
  printf("-> %s\n", result);
  if(strcmp(result, "<p class=\"hello\">Changed</p>") != 0) {
    fprintf(stderr, "Failed\n");
    html_free(result);
    MODEST_HTML_ERROR
    return 1;
  }
  html_free(result);

  return 0;
}

int get_text_from_multiple_nodes_test(html_workspace_t* w)
{
  MODEST_HTML_LOG

  const char* html = "<p>Lorem</p><p>ipsum<a href=\"http://google.de\">dolor sit</a></p>";
  const char* selector = "p,a";

  int tree_index = html_parse_tree(w, html, strlen(html));
  int selector_index = html_prepare_selector(w, selector, strlen(selector));
  const char* scope_name = "html";
  int collection_index  = html_select(w, tree_index, scope_name, selector_index);

  int text_index = html_get_text(w, collection_index);

  html_vec_str_t* text = html_get_buffer(w, text_index);
  char* result = html_vec_join(text, "|");
  printf("-> %s\n", result);
  if(strcmp(result, "Lorem|ipsum|dolor sit") != 0) {
    fprintf(stderr, "Failed\n");
    html_free(result);
    MODEST_HTML_ERROR
    return 1;
  }
  html_free(result);

  return 0;
}

int set_text_all_test(html_workspace_t* w)
{
  MODEST_HTML_LOG

  const char* html = "<p>Lorem</p><p>ipsum<a href=\"http://google.de\">dolor sit</a></p>";
  const char* selector = "p,a";

  int tree_index = html_parse_tree(w, html, strlen(html));
  int selector_index = html_prepare_selector(w, selector, strlen(selector));
  const char* scope_name = "html";
  int collection_index  = html_select(w, tree_index, scope_name, selector_index);

  html_set_text(w, collection_index, "hello");

  int buffer_index = html_serialize_collection(w, collection_index);
  html_vec_str_t* text = html_get_buffer(w, buffer_index);
  char* result = html_vec_join(text, "|");
  printf("-> %s\n", result);
  if(strcmp(result, "<p>hello</p>|<p><a href=\"http://google.de\">hello</a>hello</p>|<a href=\"http://google.de\">hello</a>") != 0) {
    fprintf(stderr, "Failed\n");
    html_free(result);
    MODEST_HTML_ERROR
    return 1;
  }
  html_free(result);

  return 0;
}

#define max_tests 3
int (*test[max_tests])() = {get_and_set_text_test, get_text_from_multiple_nodes_test, set_text_all_test};

int main(int argc, char const* argv[])
{
  html_workspace_t* w = html_init();

  int i = 0;
  int result = 0;
  while(i < max_tests && result == 0) {
    result = test[i](w);
    i += 1;
  }

  html_destroy(w);
  printf("ok\n");
  return 0;
}