#include "modest_html.h"

int insert_single_node_before_single_node_test(html_workspace_t* w)
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

  html_insert_before(w, collection_index, new_collection_index);

  int buffer_index = html_serialize_tree(w, tree_index, "body_children");
  html_vec_str_t* buffer = html_get_buffer(w, buffer_index);
  char* result = html_vec_join(buffer, "|");
  printf("-> %s\n", result);
  if(strcmp(result, "<div><span>Hello</span><p class=\"hello\">Hello</p>World</div>") != 0) {
    fprintf(stderr, "Failed\n");
    html_free(result);
    MODEST_HTML_ERROR
    return 1;
  }
  html_free(result);

  return 0;
}

int insert_text_before_single_node_test(html_workspace_t* w)
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

  html_insert_before(w, collection_index, new_collection_index);

  int buffer_index = html_serialize_tree(w, tree_index, "body_children");
  html_vec_str_t* buffer = html_get_buffer(w, buffer_index);
  char* result = html_vec_join(buffer, "|");
  printf("-> %s\n", result);
  if(strcmp(result, "<div>Lorem ipsum<p class=\"hello\">Hello</p>World</div>") != 0) {
    fprintf(stderr, "Failed\n");
    html_free(result);
    MODEST_HTML_ERROR
    return 1;
  }
  html_free(result);

  return 0;
}

#define max_tests 2
int (*test[max_tests])() = {insert_single_node_before_single_node_test, insert_text_before_single_node_test};

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

// #include "modest_html.h"

// int main(int argc, char const *argv[])
// {
//   html_workspace_t *w = html_init();
//   int i = 0;

//   // first tree

//   const char *html = "<html><head></head><body><div><p class=\"hello\">Hello</p>World</div></body></html>";
//   const char *selector = "p";
//   html_result_t s1 = html_parse_and_select(w, html, selector);

//   const char *new_html = "<span>Hello</span>";
//   const char *new_selector = "body *";
//   html_result_t new_s1 = html_parse_and_select(w, new_html, new_selector);

//   html_insert_before(w, s1.collection_index, new_s1.collection_index);
//   const char *scope_name = "body";
//   int buffer_index = html_serialize_tree(w, s1.tree_index, scope_name);
//   html_vec_str_t *buffer = html_get_buffer(w, buffer_index);
//   char *result = html_vec_join(buffer, "|");
//   printf("-> %s\n", result);
//   if(strcmp(result, "<body><div><span>Hello</span><p class=\"hello\">Hello</p>World</div></body>") != 0){
//     fprintf(stderr, "Failed\n");
//     html_free(result);
//     html_destroy(w);
//     return 1;
//   }
//   html_free(result);

//   // second tree

//   html = "<p>Lorem</p><p>ipsum<a href=\"http://google.de\">dolor sit</a></p>";
//   selector = "a";
//   html_result_t s2 = html_parse_and_select(w, html, selector);

//   new_html = "Hello World";
//   new_selector = "body *";
//   html_result_t new_s2 = html_parse_and_select(w, new_html, new_selector);

//   // TODO: check select string only

//   // html_insert_before(w, s2.collection_index, new_s2.collection_index);
//   // scope_name = "body";
//   // buffer_index = html_serialize_tree(w, s2.tree_index, scope_name);
//   // buffer = html_get_buffer(w, buffer_index);
//   // result = html_vec_join(buffer, "|");
//   // printf("-> %s\n", result);
//   // if(strcmp(result, "sdasd") != 0){
//   //   fprintf(stderr, "Failed\n");
//   //   html_free(result);
//   //   html_destroy(w);
//   //   return 1;
//   // }
//   // html_free(result);

//   html_destroy(w);
//   printf("ok\n");
//   return 0;
// }