#include "modest_html.h"
#include "test_utils.h"
/*
To check for memory leaks execute test with valgrind.

  valgrind --leak-check=yes test/example_test
*/
int serialize_large_file_test(html_workspace_t* w)
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

#define max_tests 1
int (*test[max_tests])() = {serialize_large_file_test};

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

// int main(int argc, char const* argv[])
// {
//   html_workspace_t* w = html_init();
//   int i = 0;

//   const char* html = "<html><head></head><body><div><p class=\"hello\">Hello</p>World</div></body></html>";

//   int tree_index = html_parse_tree(w, html, strlen(html));

//   const char* scope_name = "html";
//   int buffer_index = html_serialize_tree(w, tree_index, scope_name);
//   html_vec_str_t* buffer = html_get_buffer(w, buffer_index);
//   char* result = html_vec_str_join(buffer, "|");
//   printf("-> %s\n", result);
//   if(strcmp(result, "<html><head></head><body><div><p class=\"hello\">Hello</p>World</div></body></html>") != 0) {

//     html_free(result);
//     html_destroy(w);
//     return 1;
//   }
//   html_free(result);

//   html_destroy(w);
//   printf("done\n");
//   return 0;
// }