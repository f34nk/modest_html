
#include "modest_html.h"

int main(int argc, char const *argv[])
{
  html_workspace_t *w = html_init();
  int i = 0;

  // test

  const char *html = "<p>Hello World</p>";
  const char *selector = "p";

  int tree_index = html_parse_tree(w, html, strlen(html));
  int selector_index = html_prepare_selector(w, selector, strlen(selector));

  const char *scope_name = "body_children";
  int collection_index  = html_select(w, tree_index, scope_name, selector_index);


  const char *new_html = "<span>Append</span><span>Me</span>";
  int new_tree_index = html_parse_tree(w, new_html, strlen(new_html));

  scope_name = "body_children";
  int new_collection_index  = html_select_scope(w, new_tree_index, scope_name);

  html_append_collection(w, collection_index, new_collection_index);

  // serialize collection
  
  int buffer_index = html_serialize_collection(w, collection_index);
  html_vec_str_t *buffer = html_get_buffer(w, buffer_index);
  char *result = html_vec_join(buffer, "|");
  printf("%d: %s\n", ++i, result);
  if(strcmp(result, "<p>Hello World<span>Append</span><span>Me</span></p>") != 0){
    fprintf(stderr, "Failed\n");
    html_free(result);
    html_destroy(w);
    return 1;
  }
  html_free(result);

  // test

  html = "<p>Hello World</p>";
  selector = "p";

  tree_index = html_parse_tree(w, html, strlen(html));
  selector_index = html_prepare_selector(w, selector, strlen(selector));

  scope_name = "body_children";
  collection_index  = html_select(w, tree_index, scope_name, selector_index);


  new_html = "Append Me";
  new_tree_index = html_parse_tree(w, new_html, strlen(new_html));

  scope_name = "body_children";
  new_collection_index  = html_select_scope(w, new_tree_index, scope_name);

  html_append_collection(w, collection_index, new_collection_index);

  // serialize collection
  
  buffer_index = html_serialize_collection(w, collection_index);
  buffer = html_get_buffer(w, buffer_index);
  result = html_vec_join(buffer, "|");
  printf("%d: %s\n", ++i, result);
  if(strcmp(result, "<p>Hello WorldAppend Me</p>") != 0){
    fprintf(stderr, "Failed\n");
    html_free(result);
    html_destroy(w);
    return 1;
  }
  html_free(result);

  // test
  // TODO: check test case; append nodes to multiple selected target nodes
  
  // html = "<p>Hello</p><p>World</p>";
  // selector = "p";

  // tree_index = html_parse_tree(w, html, strlen(html));
  // selector_index = html_prepare_selector(w, selector, strlen(selector));

  // scope_name = "body_children";
  // collection_index  = html_select(w, tree_index, scope_name, selector_index);


  // new_html = "Append Me";
  // new_tree_index = html_parse_tree(w, new_html, strlen(new_html));

  // scope_name = "body_children";
  // new_collection_index  = html_select_scope(w, new_tree_index, scope_name);

  // html_append_collection(w, collection_index, new_collection_index);

  // // serialize collection
  
  // buffer_index = html_serialize_collection(w, collection_index);
  // buffer = html_get_buffer(w, buffer_index);
  // result = html_vec_join(buffer, "|");
  // printf("%d: %s\n", ++i, result);
  // if(strcmp(result, "<p>Hello WorldAppend Me</p>") != 0){
  //   fprintf(stderr, "Failed\n");
  //   html_free(result);
  //   html_destroy(w);
  //   return 1;
  // }
  // html_free(result);

  // test
  
  // TODO: check select string only; implement append_buffer()

  html_destroy(w);
  printf("ok\n");
  return 0;
}