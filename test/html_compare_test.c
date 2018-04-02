
#include "modest_html.h"

int main(int argc, char const *argv[])
{
  // html_workspace_t *w = html_init();
  // int i = 0;
  
  // // first tree
  
  // const char *html = "<div><p>Hello</p><p>World</p></div>";
  // const char *selector = "p";
  // html_result_t s1 = html_parse_and_select(w, html, selector);

  // html = "<div><p>Hello</p><span>world</span></div>";
  // selector = "p";
  // html_result_t s2 = html_parse_and_select(w, html, selector);

  // html_vec_int_t buffer_indices;
  // html_vec_init(&buffer_indices);
  // html_compare(w, s1.collection_index, s2.collection_index, &buffer_indices);
  
  // int j; int buffer_index;
  // html_vec_foreach(&buffer_indices, buffer_index, j) {
  //   html_vec_str_t *buffer = html_get_buffer(w, buffer_index);
  //   char *result = html_vec_join(buffer, "|");    
  //   printf("%d: %s\n", j, result);
  //   char *comparisons[2] = {
  //     "set_tag|html body div p|span",
  //     "set_text|html body div span|world"
  //   };
  //   if(strcmp(result, comparisons[j]) != 0){
  //     fprintf(stderr, "Failed\n");
  //     html_free(result);
  //     html_destroy(w);
  //     return 1;
  //   }
  //   html_free(result);
  // }
  // html_vec_clear(&buffer_indices);

  // // second tree

  // html = "<div><p>Hello</p><p id=\"change me\">World</p></div>";
  // selector = "p";
  // html_result_t s3 = html_parse_and_select(w, html, selector);

  // html = "<div><p>Hello</p><p id=\"hello\" class=\"world\">World</p></div>";
  // selector = "p";
  // html_result_t s4 = html_parse_and_select(w, html, selector);

  // html_compare(w, s3.collection_index, s4.collection_index, &buffer_indices);
  
  // html_vec_foreach(&buffer_indices, buffer_index, j) {
  //   html_vec_str_t *buffer = html_get_buffer(w, buffer_index);
  //   char *result = html_vec_join(buffer, "|");
  //   char *comparisons[2] = {
  //     "set_attribute|html body div p|class|world",
  //     "asdasd"
  //   };
  //   printf("%d: %s\n", j, result);
  //   if(strcmp(result, comparisons[j]) != 0){
  //     fprintf(stderr, "Failed\n");
  //     html_free(result);
  //     html_destroy(w);
  //     return 1;
  //   }
  //   html_free(result);
  // }
  // html_vec_deinit(&buffer_indices);

  // html_destroy(w);
  // printf("ok\n");
  return 0;
}