
#include "modest_html.h"

int compare_test(html_workspace_t* w, const char* html, const char* selector, const char* scope_name, const char* new_html, const char** instructions, int length)
{
  int tree_index = html_parse_tree(w, html, strlen(html));
  int selector_index = html_prepare_selector(w, selector, strlen(selector));
  int collection_index  = html_select(w, tree_index, scope_name, selector_index);

  int new_tree_index = html_parse_tree(w, new_html, strlen(new_html));
  const char* new_selector = selector;
  int new_selector_index = html_prepare_selector(w, new_selector, strlen(new_selector));
  const char* new_scope_name = scope_name;
  int new_collection_index  = html_select(w, new_tree_index, new_scope_name, new_selector_index);

  html_vec_int_t buffer_indices;
  html_vec_init(&buffer_indices);

  html_compare(w, collection_index, new_collection_index, &buffer_indices);

  if(buffer_indices.length != length) {
    printf("-> buffer lenght is %d but should be %d\n", buffer_indices.length, length);

    int i;
    int val;
    html_vec_foreach(&buffer_indices, val, i) {
      int buffer_index = val;
      html_vec_str_t* buffer = html_get_buffer(w, buffer_index);
      char* result = html_vec_join(buffer, "|");
      printf("%d: %s\n", i, result);
      html_free(result);
    }

    html_vec_deinit(&buffer_indices);
    MODEST_HTML_LOG_ERROR
    return 1;
  }

  // for(int i = 0; i < length; i++) {
  int i;
  int val;
  html_vec_foreach(&buffer_indices, val, i) {
    // int buffer_index = html_vec_get(buffer_indices, i);
    int buffer_index = val;
    html_vec_str_t* buffer = html_get_buffer(w, buffer_index);
    char* result = html_vec_join(buffer, "|");
    printf("%d: %s\n", i, result);
    if(strcmp(result, instructions[i]) != 0) {
      html_free(result);
      html_vec_deinit(&buffer_indices);
      MODEST_HTML_LOG_ERROR
      return 1;
    }
    html_free(result);
  }
  html_vec_deinit(&buffer_indices);

  return 0;
}

int set_tag_and_set_text_test(html_workspace_t* w)
{
  MODEST_HTML_LOG

  const char* html = "<div><p>Hello</p><p>World</p></div>";
  const char* selector = "p";
  const char* scope_name = "body_children";

  const char* new_html = "<div><p>Hello</p><span>world</span></div>";

  const char* instructions[2] = {
    "set_tag|html body div p|span",
    "set_text|html body div span|world"
  };

  return compare_test(w, html, selector, scope_name, new_html, instructions, 2);
}

int append_to_empty_body_test(html_workspace_t* w)
{
  MODEST_HTML_LOG

  const char* html = "";
  const char* selector = "body > *";
  const char* scope_name = "html";

  const char* new_html = "<div>Hello</div>";

  const char* instructions[1] = {
    "append|html body|<div>Hello</div>",
  };

  return compare_test(w, html, selector, scope_name, new_html, instructions, 1);
}

int remove_test(html_workspace_t* w)
{
  MODEST_HTML_LOG

  const char* html = "<div>Hello</div>";
  const char* selector = "body > *";
  const char* scope_name = "html";

  const char* new_html = "";

  const char* instructions[1] = {
    "remove|html body div",
  };

  return compare_test(w, html, selector, scope_name, new_html, instructions, 1);
}

int remove_and_append_test(html_workspace_t* w)
{
  MODEST_HTML_LOG

  const char* html = "<p>Hello<span>Remove me</span></p>";
  const char* selector = "p";
  const char* scope_name = "body_children";

  const char* new_html = "<p>Hello</p><p>World</p>";

  const char* instructions[2] = {
    "remove|html body p span",
    "append|html body|<p>World</p>"
  };

  return compare_test(w, html, selector, scope_name, new_html, instructions, 2);
}

int set_attribute_test(html_workspace_t* w)
{
  MODEST_HTML_LOG

  const char* html = "<div class=\"first\">Hello</div><div class=\"second\">World</div>";
  const char* selector = "body > *";
  const char* scope_name = "html";

  const char* new_html = "<div class=\"first\">Hello</div><div class=\"other\">World</div>";

  const char* instructions[1] = {
    "set_attribute|html body div|class|other"
  };

  return compare_test(w, html, selector, scope_name, new_html, instructions, 1);
}

int remove_attribute_test(html_workspace_t* w)
{
  MODEST_HTML_LOG

  const char* html = "<div class=\"other\">Hello</div>";
  const char* selector = "body > *";
  const char* scope_name = "html";

  const char* new_html = "<div>Hello</div>";

  const char* instructions[1] = {
    "set_attribute|html body div|class|"
  };

  return compare_test(w, html, selector, scope_name, new_html, instructions, 1);
}

int add_attribute_test(html_workspace_t* w)
{
  MODEST_HTML_LOG

  const char* html = "<div>Hello</div>";
  const char* selector = "body > *";
  const char* scope_name = "html";

  const char* new_html = "<div class=\"other\">Hello</div>";

  const char* instructions[1] = {
    "set_attribute|html body div|class|other"
  };

  return compare_test(w, html, selector, scope_name, new_html, instructions, 1);
}

int change_and_add_attribute_test(html_workspace_t* w)
{
  MODEST_HTML_LOG

  const char* html = "<p id=\"change-me\">World</p>";
  const char* selector = "p";
  const char* scope_name = "body_children";

  const char* new_html = "<p id=\"hello\" class=\"add-me\">World</p>";

  const char* instructions[2] = {
    "set_attribute|html body p|id|hello",
    "set_attribute|html body p|class|add-me"
  };

  return compare_test(w, html, selector, scope_name, new_html, instructions, 2);
}

int change_and_remove_attribute_test(html_workspace_t* w)
{
  MODEST_HTML_LOG

  const char* html = "<p id=\"change-me\" class=\"remove-me\">World</p>";
  const char* selector = "p";
  const char* scope_name = "body_children";

  const char* new_html = "<p id=\"hello\">World</p>";

  const char* instructions[2] = {
    "set_attribute|html body p|id|hello",
    "set_attribute|html body p|class|"
  };

  return compare_test(w, html, selector, scope_name, new_html, instructions, 2);
}

#define max_tests 9
int (*test[max_tests])() = {set_tag_and_set_text_test, remove_and_append_test, set_attribute_test, append_to_empty_body_test, remove_test, remove_attribute_test, add_attribute_test, change_and_add_attribute_test, change_and_remove_attribute_test};

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