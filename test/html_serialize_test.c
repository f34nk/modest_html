
#include "modest_html.h"
#include "test_utils.h"

int serialize_tree_and_collection_test(html_workspace_t* w)
{
  MODEST_HTML_LOG

  const char* html = "<html><head></head><body><div><p class=\"hello\">Hello</p>World</div></body></html>";
  const char* selector = "p";

  int tree_index = html_parse_tree(w, html, strlen(html));
  int selector_index = html_prepare_selector(w, selector, strlen(selector));
  const char* scope_name = "html";
  int collection_index  = html_select(w, tree_index, scope_name, selector_index);

  int buffer_index = html_serialize_tree(w, tree_index, scope_name);
  html_vec_str_t* buffer = html_get_buffer(w, buffer_index);
  char* result = html_vec_str_join(buffer, "|");
  printf("-> %s\n", result);
  if(strcmp(result, "<html><head></head><body><div><p class=\"hello\">Hello</p>World</div></body></html>") != 0) {
    html_free(result);
    MODEST_HTML_LOG_ERROR
    return 1;
  }
  html_free(result);

  buffer_index = html_serialize_collection(w, collection_index);
  buffer = html_get_buffer(w, buffer_index);
  result = html_vec_str_join(buffer, "|");
  printf("-> %s\n", result);
  if(strcmp(result, "<p class=\"hello\">Hello</p>") != 0) {
    html_free(result);
    MODEST_HTML_LOG_ERROR
    return 1;
  }
  html_free(result);

  return 0;
}

int serialize_tree_with_body_children_test(html_workspace_t* w)
{
  MODEST_HTML_LOG

  const char* html = "<p>Hello</p><p>World</p>";

  int tree_index = html_parse_tree(w, html, strlen(html));

  int buffer_index = html_serialize_tree(w, tree_index, "body_children");
  html_vec_str_t* buffer = html_get_buffer(w, buffer_index);
  char* result = html_vec_str_join(buffer, "");
  printf("-> %s\n", result);
  if(strcmp(result, "<p>Hello</p><p>World</p>") != 0) {
    html_free(result);
    MODEST_HTML_LOG_ERROR
    return 1;
  }
  html_free(result);

  return 0;
}

int serialize_empty_tree_test(html_workspace_t* w)
{
  MODEST_HTML_LOG

  const char* html = "";
  const char* scope_name = "html";

  int tree_index = html_parse_tree(w, html, strlen(html));

  int buffer_index = html_serialize_tree(w, tree_index, scope_name);
  html_vec_str_t* buffer = html_get_buffer(w, buffer_index);
  char* result = html_vec_str_join(buffer, "|");
  printf("-> %s\n", result);
  if(strcmp(result, "<html><head></head><body></body></html>") != 0) {
    html_free(result);
    MODEST_HTML_LOG_ERROR
    return 1;
  }
  html_free(result);

  return 0;
}

int serialize_body_children_text_only_test(html_workspace_t* w)
{
  MODEST_HTML_LOG

  const char* html = "Hello World";
  const char* selector = "p";

  int tree_index = html_parse_tree(w, html, strlen(html));
  int selector_index = html_prepare_selector(w, selector, strlen(selector));
  const char* scope_name = "html";
  int collection_index  = html_select(w, tree_index, scope_name, selector_index);

  int buffer_index = html_serialize_tree(w, tree_index, "body_children");
  html_vec_str_t* buffer = html_get_buffer(w, buffer_index);
  char* result = html_vec_str_join(buffer, "|");
  printf("-> %s\n", result);
  if(strcmp(result, "Hello World") != 0) {
    html_free(result);
    MODEST_HTML_LOG_ERROR
    return 1;
  }
  html_free(result);

  return 0;
}

int serialize_node_test(html_workspace_t* w)
{
  MODEST_HTML_LOG

  const char* html = "<div>Hello</div><div><span><p><a>World</a></p></span></div>";
  const char* selector = "a";

  int tree_index = html_parse_tree(w, html, strlen(html));
  int selector_index = html_prepare_selector(w, selector, strlen(selector));
  const char* scope_name = "html";
  int collection_index  = html_select(w, tree_index, scope_name, selector_index);

  myhtml_collection_t* collection = (myhtml_collection_t*)html_get_collection(w, collection_index);
  myhtml_tree_node_t* node = collection->list[0];

  char* result = html_serialize_node(node);
  printf("-> %s\n", result);
  if(strcmp(result, "<a>World</a>") != 0) {
    html_free(result);
    MODEST_HTML_LOG_ERROR
    return 1;
  }
  html_free(result);
  // free(result);

  return 0;
}

int serialize_selector_test(html_workspace_t* w)
{
  MODEST_HTML_LOG

  const char* html = "<div>Hello</div><div><span><p><a>World</a></p></span></div>";
  const char* selector = "a";

  int tree_index = html_parse_tree(w, html, strlen(html));
  int selector_index = html_prepare_selector(w, selector, strlen(selector));
  const char* scope_name = "html";
  int collection_index  = html_select(w, tree_index, scope_name, selector_index);

  myhtml_collection_t* collection = (myhtml_collection_t*)html_get_collection(w, collection_index);
  myhtml_tree_node_t* node = collection->list[0];

  char* result = html_serialize_selector(node);
  printf("-> %s\n", result);
  if(strcmp(result, "html body div span p a") != 0) {
    html_free(result);
    MODEST_HTML_LOG_ERROR
    return 1;
  }
  html_free(result);

  return 0;
}

int load_file_and_serialize(html_workspace_t* w)
{
  MODEST_HTML_LOG

  // const char* filename = "../test/fixtures/0_2k.html";
  // const char* filename = "../test/fixtures/0_5k.html";
  // const char* filename = "../test/fixtures/1k.html";
  const char* filename = "../test/fixtures/2k.html";

  char* html = read_file(filename);
  if(html == NULL) {
    MODEST_HTML_LOG_ERROR
    return 1;
  }

  int tree_index = html_parse_tree(w, html, strlen(html));
  const char* scope_name = "html";
  int buffer_index = html_serialize_tree(w, tree_index, scope_name);
  html_vec_str_t* buffer = html_get_buffer(w, buffer_index);
  char* result = html_vec_str_join(buffer, "|");
  printf("-> read size = %d, serialized size = %d\n", strlen(html), strlen(result));

  // write_file("result.html", result);
  // write_file("input.html", html);

  if(strlen(result) < strlen(html)) {
    html_free(result);
    free(html);
    MODEST_HTML_LOG_ERROR
    return 1;
  }
  html_free(result);
  
  free(html);

  return 0;
}

#define max_tests 7
int (*test[max_tests])() = {serialize_tree_and_collection_test, serialize_tree_with_body_children_test, serialize_empty_tree_test, serialize_body_children_text_only_test, serialize_node_test, serialize_selector_test, load_file_and_serialize};

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