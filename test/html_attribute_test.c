
#include "modest_html.h"

int get_and_set_attribute_for_single_node_test(html_workspace_t* w)
{
  MODEST_HTML_LOG
  const char* html = "<html><head></head><body><div><p class=\"hello\">Hello</p>World</div></body></html>";
  const char* selector = "p";

  int tree_index = html_parse_tree(w, html, strlen(html));
  int selector_index = html_prepare_selector(w, selector, strlen(selector));
  const char* scope_name = "body_children";
  int collection_index  = html_select(w, tree_index, scope_name, selector_index);

  int attributes_index = html_get_attribute(w, collection_index, "class");

  html_vec_str_t* attributes = html_get_buffer(w, attributes_index);
  char* result = html_vec_join(attributes, "|");
  printf("-> %s\n", result);
  if(strcmp(result, "hello") != 0) {
    fprintf(stderr, "Failed\n");
    html_free(result);
    MODEST_HTML_ERROR
    return 1;
  }
  html_free(result);

  html_set_attribute(w, collection_index, "class", "changed");

  int buffer_index = html_serialize_collection(w, collection_index);
  html_vec_str_t* buffer = html_get_buffer(w, buffer_index);
  result = html_vec_join(buffer, "|");
  printf("-> %s\n", result);
  if(strcmp(result, "<p class=\"changed\">Hello</p>") != 0) {
    fprintf(stderr, "Failed\n");
    html_free(result);
    MODEST_HTML_ERROR
    return 1;
  }
  html_free(result);

  return 0;
}

int set_new_attribute_for_single_node_test(html_workspace_t* w)
{
  MODEST_HTML_LOG
  const char* html = "<html><head></head><body><div><p>Hello</p>World</div></body></html>";
  const char* selector = "p";

  int tree_index = html_parse_tree(w, html, strlen(html));
  int selector_index = html_prepare_selector(w, selector, strlen(selector));
  const char* scope_name = "body_children";
  int collection_index  = html_select(w, tree_index, scope_name, selector_index);

  html_set_attribute(w, collection_index, "class", "new-class");

  int buffer_index = html_serialize_collection(w, collection_index);
  html_vec_str_t* buffer = html_get_buffer(w, buffer_index);
  char* result = html_vec_join(buffer, "|");
  printf("-> %s\n", result);
  if(strcmp(result, "<p class=\"new-class\">Hello</p>") != 0) {
    fprintf(stderr, "Failed\n");
    html_free(result);
    MODEST_HTML_ERROR
    return 1;
  }
  html_free(result);

  return 0;
}

#define max_tests 2
int (*test[max_tests])() = {get_and_set_attribute_for_single_node_test, set_new_attribute_for_single_node_test};

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