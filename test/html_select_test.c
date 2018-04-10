
#include "modest_html.h"

int select_single_node_test(html_workspace_t* w)
{
  MODEST_HTML_LOG

  const char* html = "<html><head></head><body><div><p class=\"hello\">Hello</p>World</div></body></html>";
  const char* selector = "p";

  int tree_index = html_parse_tree(w, html, strlen(html));
  int selector_index = html_prepare_selector(w, selector, strlen(selector));

  const char* scope_name = "html";
  int collection_index  = html_select(w, tree_index, scope_name, selector_index);

  int buffer_index = html_serialize_collection(w, collection_index);
  html_vec_str_t* buffer = html_get_buffer(w, buffer_index);
  char* result = html_vec_join(buffer, "|");
  printf("-> %s\n", result);
  if(strcmp(result, "<p class=\"hello\">Hello</p>") != 0) {
    html_free(result);
    MODEST_HTML_LOG_ERROR
    return 1;
  }
  html_free(result);
  return 0;
}

int different_selector_same_result_test(html_workspace_t* w)
{
  MODEST_HTML_LOG

  const char* html = "Lorem<p>ipsum</p><span>dolor</span>";
  const char* selector = "body > *";

  int tree_index = html_parse_tree(w, html, strlen(html));
  int selector_index = html_prepare_selector(w, selector, strlen(selector));

  const char* scope_name = "body";
  int collection_index  = html_select(w, tree_index, scope_name, selector_index);

  int buffer_index = html_serialize_collection(w, collection_index);
  html_vec_str_t* buffer = html_get_buffer(w, buffer_index);
  char* result = html_vec_join(buffer, "|");
  printf("-> %s\n", result);
  if(strcmp(result, "<p>ipsum</p>|<span>dolor</span>") != 0) {
    html_free(result);
    MODEST_HTML_LOG_ERROR
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
  printf("-> %s\n", result);
  if(strcmp(result, "<p>ipsum</p>|<span>dolor</span>") != 0) {
    html_free(result);
    MODEST_HTML_LOG_ERROR
    return 1;
  }
  html_free(result);

  return 0;
}

int asterix_selector_edge_cases_test(html_workspace_t* w)
{
  MODEST_HTML_LOG

  const char* html = "Hello World";
  const char* selector = "*";

  int tree_index = html_parse_tree(w, html, strlen(html));
  if(tree_index == -1) {
    fprintf(stderr, "Failed: tree_index = %d\n", tree_index);
    MODEST_HTML_LOG_ERROR
    return 1;
  }
  int selector_index = html_prepare_selector(w, selector, strlen(selector));
  if(selector_index == -1) {
    fprintf(stderr, "Failed: selector_index = %d\n", selector_index);
    MODEST_HTML_LOG_ERROR
    return 1;
  }

  const char* scope_name = "body";
  int collection_index  = html_select(w, tree_index, scope_name, selector_index);
  printf("-> collection_index = %d\n", collection_index);
  if(collection_index == -1) {
    MODEST_HTML_LOG_ERROR
    return 1;
  }

  /*
  html_select()
  The asterix selector fails for this scope.
   */
  scope_name = "body_children";
  collection_index  = html_select(w, tree_index, scope_name, selector_index);
  printf("-> collection_index = %d\n", collection_index);
  if(collection_index != -1) {
    MODEST_HTML_LOG_ERROR
    return 1;
  }

  /*
  html_select_scope()
  Selecting the scope returns a new collection of all nodes in the scope.
   */
  scope_name = "body_children";
  collection_index  = html_select_scope(w, tree_index, scope_name);
  printf("-> collection_index = %d\n", collection_index);
  if(collection_index == -1) {
    MODEST_HTML_LOG_ERROR
    return 1;
  }

  int buffer_index = html_serialize_collection(w, collection_index);
  html_vec_str_t* buffer = html_get_buffer(w, buffer_index);
  char* result = html_vec_join(buffer, "|");
  printf("-> %s\n", result);
  if(strcmp(result, "Hello World") != 0) {
    html_free(result);
    MODEST_HTML_LOG_ERROR
    return 1;
  }
  html_free(result);

  return 0;
}

#define max_tests 3
int (*test[max_tests])() = {select_single_node_test, different_selector_same_result_test, asterix_selector_edge_cases_test};

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
  printf("done\n");
  return result;
}