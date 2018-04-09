
#include "modest_html.h"

int set_tag_and_set_text_test(html_workspace_t* w)
{
  MODEST_HTML_LOG

  const char* html = "<div><p>Hello</p><p>World</p></div>";
  const char* selector = "p";
  int tree_index = html_parse_tree(w, html, strlen(html));
  int selector_index = html_prepare_selector(w, selector, strlen(selector));
  const char* scope_name = "body_children";
  int collection_index  = html_select(w, tree_index, scope_name, selector_index);

  const char* new_html = "<div><p>Hello</p><span>world</span></div>";
  const char* new_selector = "p";
  int new_tree_index = html_parse_tree(w, new_html, strlen(new_html));
  int new_selector_index = html_prepare_selector(w, new_selector, strlen(new_selector));
  const char* new_scope_name = "body_children";
  int new_collection_index  = html_select(w, new_tree_index, new_scope_name, new_selector_index);

  char* instructions[2] = {
    "set_tag|html body div p|span",
    "set_text|html body div span|world"
  };

  html_vec_int_t buffer_indices;
  html_vec_init(&buffer_indices);

  html_compare(w, collection_index, new_collection_index, &buffer_indices);

  for(int i = 0; i < 2; i++) {
    int buffer_index = html_vec_get(buffer_indices, i);
    html_vec_str_t* buffer = html_get_buffer(w, buffer_index);
    char* result = html_vec_join(buffer, "|");
    printf("%d: %s\n", i, result);
    if(strcmp(result, instructions[i]) != 0) {
      fprintf(stderr, "Failed\n");
      html_free(result);
      html_vec_deinit(&buffer_indices);
      MODEST_HTML_ERROR
      return 1;
    }
    html_free(result);
  }
  html_vec_deinit(&buffer_indices);

  return 0;
}

int remove_and_append_test(html_workspace_t* w)
{
  MODEST_HTML_LOG

  const char* html = "<p>Hello<span>Remove me</span></p>";
  const char* selector = "p";
  int tree_index = html_parse_tree(w, html, strlen(html));
  int selector_index = html_prepare_selector(w, selector, strlen(selector));
  const char* scope_name = "body_children";
  int collection_index  = html_select(w, tree_index, scope_name, selector_index);

  const char* new_html = "<p>Hello</p><p>World</p>";
  const char* new_selector = "p";
  int new_tree_index = html_parse_tree(w, new_html, strlen(new_html));
  int new_selector_index = html_prepare_selector(w, new_selector, strlen(new_selector));
  const char* new_scope_name = "body_children";
  int new_collection_index  = html_select(w, new_tree_index, new_scope_name, new_selector_index);

  char* instructions[2] = {
    "remove|html body p span",
    "append|html body|<p>World</p>"
  };

  html_vec_int_t buffer_indices;
  html_vec_init(&buffer_indices);

  html_compare(w, collection_index, new_collection_index, &buffer_indices);

  for(int i = 0; i < 2; i++) {
    int buffer_index = html_vec_get(buffer_indices, i);
    html_vec_str_t* buffer = html_get_buffer(w, buffer_index);
    char* result = html_vec_join(buffer, "|");
    printf("%d: %s\n", i, result);
    if(strcmp(result, instructions[i]) != 0) {
      fprintf(stderr, "Failed\n");
      html_free(result);
      html_vec_deinit(&buffer_indices);
      MODEST_HTML_ERROR
      return 1;
    }
    html_free(result);
  }
  html_vec_deinit(&buffer_indices);

  return 0;
}

int change_and_add_attribute_test(html_workspace_t* w)
{
  MODEST_HTML_LOG

  const char* html = "<p id=\"change-me\">World</p>";
  const char* selector = "p";
  int tree_index = html_parse_tree(w, html, strlen(html));
  int selector_index = html_prepare_selector(w, selector, strlen(selector));
  const char* scope_name = "body_children";
  int collection_index  = html_select(w, tree_index, scope_name, selector_index);

  const char* new_html = "<p id=\"hello\" class=\"add-me\">World</p>";
  const char* new_selector = "p";
  int new_tree_index = html_parse_tree(w, new_html, strlen(new_html));
  int new_selector_index = html_prepare_selector(w, new_selector, strlen(new_selector));
  const char* new_scope_name = "body_children";
  int new_collection_index  = html_select(w, new_tree_index, new_scope_name, new_selector_index);

  char* instructions[2] = {
    "set_attribute|html body p|id|hello",
    "set_attribute|html body p|class|add-me"
  };

  html_vec_int_t buffer_indices;
  html_vec_init(&buffer_indices);
  html_compare(w, collection_index, new_collection_index, &buffer_indices);

  for(int i = 0; i < 2; i++) {
    int buffer_index = html_vec_get(buffer_indices, i);
    html_vec_str_t* buffer = html_get_buffer(w, buffer_index);
    char* result = html_vec_join(buffer, "|");
    printf("%d: %s\n", i, result);
    if(strcmp(result, instructions[i]) != 0) {
      fprintf(stderr, "Failed\n");
      html_free(result);
      html_vec_deinit(&buffer_indices);
      MODEST_HTML_ERROR
      return 1;
    }
    html_free(result);
  }
  html_vec_deinit(&buffer_indices);

  return 0;
}

int change_and_remove_attribute_test(html_workspace_t* w)
{
  MODEST_HTML_LOG

  const char* html = "<p id=\"change-me\" class=\"remove-me\">World</p>";
  const char* selector = "p";
  int tree_index = html_parse_tree(w, html, strlen(html));
  int selector_index = html_prepare_selector(w, selector, strlen(selector));
  const char* scope_name = "body_children";
  int collection_index  = html_select(w, tree_index, scope_name, selector_index);

  const char* new_html = "<p id=\"hello\">World</p>";
  const char* new_selector = "p";
  int new_tree_index = html_parse_tree(w, new_html, strlen(new_html));
  int new_selector_index = html_prepare_selector(w, new_selector, strlen(new_selector));
  const char* new_scope_name = "body_children";
  int new_collection_index  = html_select(w, new_tree_index, new_scope_name, new_selector_index);

  char* instructions[2] = {
    "set_attribute|html body p|id|hello",
    "set_attribute|html body p|class|"
  };

  html_vec_int_t buffer_indices;
  html_vec_init(&buffer_indices);
  html_compare(w, collection_index, new_collection_index, &buffer_indices);

  for(int i = 0; i < 2; i++) {
    int buffer_index = html_vec_get(buffer_indices, i);
    html_vec_str_t* buffer = html_get_buffer(w, buffer_index);
    char* result = html_vec_join(buffer, "|");
    printf("%d: %s\n", i, result);
    if(strcmp(result, instructions[i]) != 0) {
      fprintf(stderr, "Failed\n");
      html_free(result);
      html_vec_deinit(&buffer_indices);
      MODEST_HTML_ERROR
      return 1;
    }
    html_free(result);
  }
  html_vec_deinit(&buffer_indices);

  return 0;
}

#define max_tests 4
int (*test[max_tests])() = {set_tag_and_set_text_test, remove_and_append_test, change_and_add_attribute_test, change_and_remove_attribute_test};

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