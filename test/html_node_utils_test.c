#include "modest_html.h"

int main(int argc, char const *argv[])
{
  html_workspace_t *w = html_init();
  int i = 0;

  // first tree
  
  const char *html = "<p class=\"hello\">Hello</p>";
  const char *selector = "p";
  html_result_t s1 = html_parse_and_select(w, html, selector);

  myhtml_tree_node_t *node = html_get_node(w, s1.collection_index, 0);

  if(html_node_is_text(node) == true) {
    fprintf(stderr, "Failed\n");
    html_destroy(w);
    return 1;
  }

  if(html_node_has_attributes(node) == false) {
    fprintf(stderr, "Failed\n");
    html_destroy(w);
    return 1;
  }

  html_vec_int_t buffer_indices;
  html_vec_init(&buffer_indices);
  html_node_get_attributes(w, node, &buffer_indices);
  if(buffer_indices.length != 1) {
    fprintf(stderr, "Failed\n");
    html_vec_deinit(&buffer_indices);
    html_destroy(w);
    return 1;
  }

  html_vec_str_t *attributes = html_get_buffer(w, buffer_indices.data[0]);
  char *result = html_vec_join(attributes, "=");
  printf("%d: %s\n", ++i, result);
  if(strcmp(result, "class=hello") != 0){
    fprintf(stderr, "Failed\n");
    html_free(result);
    html_vec_deinit(&buffer_indices);
    html_destroy(w);
    return 1;
  }
  html_free(result);
  html_vec_deinit(&buffer_indices);

  html_destroy(w);

  return 0;
}