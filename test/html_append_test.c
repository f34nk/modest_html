
#include "modest_html.h"

int append_multiple_nodes_to_single_node_test(html_workspace_t* w)
{
  MODEST_HTML_LOG
  const char* html = "<p>Hello World</p>";
  const char* selector = "p";

  int tree_index = html_parse_tree(w, html, strlen(html));
  int selector_index = html_prepare_selector(w, selector, strlen(selector));
  const char* scope_name = "body_children";
  int collection_index  = html_select(w, tree_index, scope_name, selector_index);

  const char* new_html = "<span>Append</span><span>Me</span>";
  int new_tree_index = html_parse_tree(w, new_html, strlen(new_html));
  const char* new_scope_name = "body_children";
  int new_collection_index  = html_select_scope(w, new_tree_index, new_scope_name);

  html_append_collection(w, collection_index, new_collection_index);

  int buffer_index = html_serialize_collection(w, collection_index);
  html_vec_str_t* buffer = html_get_buffer(w, buffer_index);
  char* result = html_vec_str_join(buffer, "|");
  printf("-> %s\n", result);
  if(strcmp(result, "<p>Hello World<span>Append</span><span>Me</span></p>") != 0) {
    html_free(result);
    MODEST_HTML_LOG_ERROR
    return 1;
  }
  html_free(result);

  return 0;
}

int append_multiple_nodes_to_multiple_nodes_test(html_workspace_t* w)
{
  MODEST_HTML_LOG
  const char* html = "<p>Hello</p><p>World</p>";
  const char* selector = "p";

  int tree_index = html_parse_tree(w, html, strlen(html));
  int selector_index = html_prepare_selector(w, selector, strlen(selector));
  const char* scope_name = "body_children";
  int collection_index  = html_select(w, tree_index, scope_name, selector_index);

  const char* new_html = "<span>Append</span><span>Me</span>";
  int new_tree_index = html_parse_tree(w, new_html, strlen(new_html));
  const char* new_scope_name = "body_children";
  int new_collection_index  = html_select_scope(w, new_tree_index, new_scope_name);

  bool result = html_append_collection(w, collection_index, new_collection_index);
  if(result == true) {
    MODEST_HTML_LOG_ERROR
    return 1;
  }

  // int buffer_index = html_serialize_collection(w, collection_index);
  // html_vec_str_t* buffer = html_get_buffer(w, buffer_index);
  // char* result = html_vec_str_join(buffer, "|");
  // printf("-> %s\n", result);
  // if(strcmp(result, "<p>Hello<span>Append</span><span>Me</span></p><p>World<span>Append</span><span>Me</span></p>") != 0) {
  //
  //   html_free(result);
  //   MODEST_HTML_LOG_ERROR
  //   return 1;
  // }
  // html_free(result);

  return 0;
}

int append_text_node_to_single_node_test(html_workspace_t* w)
{
  MODEST_HTML_LOG
  const char* html = "<p>Hello World</p>";
  const char* selector = "p";

  int tree_index = html_parse_tree(w, html, strlen(html));
  int selector_index = html_prepare_selector(w, selector, strlen(selector));
  const char* scope_name = "body_children";
  int collection_index  = html_select(w, tree_index, scope_name, selector_index);

  const char* new_html = "Append Me";
  int new_tree_index = html_parse_tree(w, new_html, strlen(new_html));
  const char* new_scope_name = "body_children";
  int new_collection_index  = html_select_scope(w, new_tree_index, new_scope_name);

  html_append_collection(w, collection_index, new_collection_index);

  int buffer_index = html_serialize_collection(w, collection_index);
  html_vec_str_t* buffer = html_get_buffer(w, buffer_index);
  char* result = html_vec_str_join(buffer, "|");
  printf("-> %s\n", result);
  if(strcmp(result, "<p>Hello WorldAppend Me</p>") != 0) {
    html_free(result);
    MODEST_HTML_LOG_ERROR
    return 1;
  }
  html_free(result);

  return 0;
}

#define max_tests 3
int (*test[max_tests])() = {append_multiple_nodes_to_single_node_test, append_multiple_nodes_to_multiple_nodes_test, append_text_node_to_single_node_test};

int main(int argc, char const* argv[])
{
  html_workspace_t* w = html_init();

  int i = 0;
  int result = 0;
  while(i < max_tests && result == 0) {
    result = test[i](w);
    i += 1;
  }

  // TODO: check test case; append nodes to multiple selected target nodes

  // TODO: check select string only; implement append_buffer()

  html_destroy(w);

  printf("done\n");
  return result;
}