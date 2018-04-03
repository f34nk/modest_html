
#include "modest_html.h"

int main(int argc, char const *argv[])
{
  html_workspace_t *w = html_init();
  int i = 0;
  
  // test
  
  const char *html = "<div><p>Hello</p><p>World</p></div>";
  const char *selector = "p";
  html_result_t s1 = html_parse_and_select(w, html, selector);

  html = "<div><p>Hello</p><span>world</span></div>";
  selector = "p";
  html_result_t s2 = html_parse_and_select(w, html, selector);

  html_vec_int_t buffer_indices;
  html_vec_init(&buffer_indices);
  html_compare(w, s1.collection_index, s2.collection_index, &buffer_indices);
  
  int j; int buffer_index;
  html_vec_foreach(&buffer_indices, buffer_index, j) {
    html_vec_str_t *buffer = html_get_buffer(w, buffer_index);
    char *result = html_vec_join(buffer, "|");
    printf("%d: %s\n", j, result);
    char *comparisons[2] = {
      "set_tag|html body div p|span",
      "set_text|html body div span|world"
    };
    if(strcmp(result, comparisons[j]) != 0){
      fprintf(stderr, "Failed\n");
      html_free(result);
      html_destroy(w);
      return 1;
    }
    html_free(result);
  }
  html_vec_clear(&buffer_indices);

  // test

  html = "<p>Hello<span>Remove me</span></p>";
  selector = "p";
  html_result_t s3 = html_parse_and_select(w, html, selector);

  html = "<p>Hello</p><p>World</p>";
  selector = "p";
  html_result_t s4 = html_parse_and_select(w, html, selector);

  html_compare(w, s3.collection_index, s4.collection_index, &buffer_indices);
  
  html_vec_foreach(&buffer_indices, buffer_index, j) {
    html_vec_str_t *buffer = html_get_buffer(w, buffer_index);
    char *result = html_vec_join(buffer, "|");
    char *comparisons[2] = {
      "remove|html body p span",
      "append|html body|<p>World</p>"
    };
    printf("%d: %s\n", j, result);
    if(strcmp(result, comparisons[j]) != 0){
      fprintf(stderr, "Failed\n");
      html_free(result);
      html_destroy(w);
      return 1;
    }
    html_free(result);
  }
  html_vec_clear(&buffer_indices);

  // test
  
  html = "<p>Hello</p><p id=\"change-me\">World</p>";
  selector = "p";
  html_result_t s5 = html_parse_and_select(w, html, selector);

  html = "<p>Hello</p><p id=\"hello\">World</p>";
  selector = "p";
  html_result_t s6 = html_parse_and_select(w, html, selector);

  html_compare(w, s5.collection_index, s6.collection_index, &buffer_indices);
  
  html_vec_foreach(&buffer_indices, buffer_index, j) {
    html_vec_str_t *buffer = html_get_buffer(w, buffer_index);
    char *result = html_vec_join(buffer, "|");
    char *comparisons[1] = {
      "set_attribute|html body p|id|hello"
    };
    printf("%d: %s\n", j, result);
    if(strcmp(result, comparisons[j]) != 0){
      fprintf(stderr, "Failed\n");
      html_free(result);
      html_destroy(w);
      return 1;
    }
    html_free(result);
  }
  html_vec_clear(&buffer_indices);

  // test
  
  html = "<p id=\"hello\">World</p>";
  selector = "p";
  html_result_t s7 = html_parse_and_select(w, html, selector);

  html = "<p id=\"hello\" class=\"add-me\">World</p>";
  selector = "p";
  html_result_t s8 = html_parse_and_select(w, html, selector);

  html_compare(w, s7.collection_index, s8.collection_index, &buffer_indices);
  
  html_vec_foreach(&buffer_indices, buffer_index, j) {
    html_vec_str_t *buffer = html_get_buffer(w, buffer_index);
    char *result = html_vec_join(buffer, "|");
    char *comparisons[1] = {
      "set_attribute|html body p|class|add-me"
    };
    printf("%d: %s\n", j, result);
    if(strcmp(result, comparisons[j]) != 0){
      fprintf(stderr, "Failed\n");
      html_free(result);
      html_destroy(w);
      return 1;
    }
    html_free(result);
  }
  html_vec_clear(&buffer_indices);

  // test
  
  html = "<p id=\"change-me\" class=\"remove-me\">World</p>";
  selector = "p";
  html_result_t s9 = html_parse_and_select(w, html, selector);

  html = "<p id=\"hello\">World</p>";
  selector = "p";
  html_result_t s10 = html_parse_and_select(w, html, selector);

  html_compare(w, s9.collection_index, s10.collection_index, &buffer_indices);
  
  html_vec_foreach(&buffer_indices, buffer_index, j) {
    html_vec_str_t *buffer = html_get_buffer(w, buffer_index);
    char *result = html_vec_join(buffer, "|");
    char *comparisons[2] = {
      "set_attribute|html body p|id|hello",
      "set_attribute|html body p|class|"
    };
    printf("%d: %s\n", j, result);
    if(strcmp(result, comparisons[j]) != 0){
      fprintf(stderr, "Failed\n");
      html_free(result);
      html_destroy(w);
      return 1;
    }
    html_free(result);
  }
  html_vec_clear(&buffer_indices);


  html_vec_deinit(&buffer_indices);

  html_destroy(w);
  printf("ok\n");
  return 0;
}