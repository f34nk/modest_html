
#include "modest_html.h"

int insert_single_node_after_single_node_test(html_workspace_t* w)
{
  MODEST_HTML_LOG

  const char* html = "<html><head></head><body><div><p class=\"hello\">Hello</p>World</div></body></html>";
  const char* selector = "p";
  int tree_index = html_parse_tree(w, html, strlen(html));
  int selector_index = html_prepare_selector(w, selector, strlen(selector));
  const char* scope_name = "body_children";
  int collection_index  = html_select(w, tree_index, scope_name, selector_index);

  const char* new_html = "<span>Hello</span>";
  int new_tree_index = html_parse_tree(w, new_html, strlen(new_html));
  const char* new_scope_name = "body_children";
  int new_collection_index  = html_select_scope(w, new_tree_index, new_scope_name);

  html_insert_after(w, collection_index, new_collection_index);

  int buffer_index = html_serialize_tree(w, tree_index, "body_children");
  html_vec_str_t* buffer = html_get_buffer(w, buffer_index);
  char* result = html_vec_join(buffer, "|");
  printf("-> %s\n", result);
  if(strcmp(result, "<div><p class=\"hello\">Hello</p><span>Hello</span>World</div>") != 0) {
    html_free(result);
    MODEST_HTML_LOG_ERROR
    return 1;
  }
  html_free(result);

  return 0;
}

int insert_multiple_nodes_after_single_node_test(html_workspace_t* w)
{
  MODEST_HTML_LOG

  const char* html = "<p>Hello World</p>";
  const char* selector = "p";
  int tree_index = html_parse_tree(w, html, strlen(html));
  int selector_index = html_prepare_selector(w, selector, strlen(selector));
  const char* scope_name = "body_children";
  int collection_index  = html_select(w, tree_index, scope_name, selector_index);

  const char* new_html = "<span>Insert</span><span>Me</span>";
  int new_tree_index = html_parse_tree(w, new_html, strlen(new_html));
  const char* new_scope_name = "body_children";
  int new_collection_index  = html_select_scope(w, new_tree_index, new_scope_name);

  html_insert_after(w, collection_index, new_collection_index);

  int buffer_index = html_serialize_tree(w, tree_index, "body_children");
  html_vec_str_t* buffer = html_get_buffer(w, buffer_index);
  char* result = html_vec_join(buffer, "|");
  printf("-> %s\n", result);
  if(strcmp(result, "<p>Hello World</p>|<span>Insert</span>|<span>Me</span>") != 0) {
    html_free(result);
    MODEST_HTML_LOG_ERROR
    return 1;
  }
  html_free(result);

  return 0;
}

int insert_multiple_nodes_after_multiple_nodes_test(html_workspace_t* w)
{
  MODEST_HTML_LOG

  const char* html = "<p>Hello</p><p>World</p>";
  const char* selector = "p";
  int tree_index = html_parse_tree(w, html, strlen(html));
  int selector_index = html_prepare_selector(w, selector, strlen(selector));
  const char* scope_name = "body_children";
  int collection_index  = html_select(w, tree_index, scope_name, selector_index);

  const char* new_html = "<span>Insert</span><span>Me</span>";
  int new_tree_index = html_parse_tree(w, new_html, strlen(new_html));
  const char* new_scope_name = "body_children";
  int new_collection_index  = html_select_scope(w, new_tree_index, new_scope_name);

  bool result = html_insert_after(w, collection_index, new_collection_index);
  if(result == true) {
    MODEST_HTML_LOG_ERROR
    return 1;
  }
  
  // int buffer_index = html_serialize_tree(w, tree_index, "body_children");
  // html_vec_str_t* buffer = html_get_buffer(w, buffer_index);
  // char* result = html_vec_join(buffer, "|");
  // printf("-> %s\n", result);
  // if(strcmp(result, "<p>Hello</p><span>Insert</span><span>Me</span><p>World</p><span>Insert</span><span>Me</span>") != 0) {
  //   
  //   html_free(result);
  //   MODEST_HTML_LOG_ERROR
  //   return 1;
  // }
  // html_free(result);

  return 0;
}

int insert_text_after_single_node_test(html_workspace_t* w)
{
  MODEST_HTML_LOG

  const char* html = "<html><head></head><body><div><p class=\"hello\">Hello</p>World</div></body></html>";
  const char* selector = "p";
  int tree_index = html_parse_tree(w, html, strlen(html));
  int selector_index = html_prepare_selector(w, selector, strlen(selector));
  const char* scope_name = "body_children";
  int collection_index  = html_select(w, tree_index, scope_name, selector_index);

  const char* new_html = "Lorem ipsum";
  int new_tree_index = html_parse_tree(w, new_html, strlen(new_html));
  const char* new_scope_name = "body_children";
  int new_collection_index  = html_select_scope(w, new_tree_index, new_scope_name);

  html_insert_after(w, collection_index, new_collection_index);

  int buffer_index = html_serialize_tree(w, tree_index, "body_children");
  html_vec_str_t* buffer = html_get_buffer(w, buffer_index);
  char* result = html_vec_join(buffer, "|");
  printf("-> %s\n", result);
  if(strcmp(result, "<div><p class=\"hello\">Hello</p>Lorem ipsumWorld</div>") != 0) {
    html_free(result);
    MODEST_HTML_LOG_ERROR
    return 1;
  }
  html_free(result);

  return 0;
}

#define max_tests 4
int (*test[max_tests])() = {insert_single_node_after_single_node_test, insert_multiple_nodes_after_single_node_test, insert_multiple_nodes_after_multiple_nodes_test, insert_text_after_single_node_test};

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