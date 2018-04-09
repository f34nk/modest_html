#include "modest_html.h"
/*
To check for memory leaks execute test with valgrind.

  valgrind --leak-check=yes test/html_vec_test

*/
int main(int argc, char const* argv[])
{
  int i = 0;
  html_map_str_t m;
  html_map_init(&m);

  char* copy;
  html_string_copy("a 123", copy);

  html_map_set(&m, "a", copy);

  char* result = (char*)*html_map_get(&m, "a");
  printf("-> %s\n", result);
  if(strcmp(result, "a 123") != 0) {
    fprintf(stderr, "Failed\n");
    html_free(result);
    html_map_deinit(&m);
    MODEST_HTML_ERROR
    return 1;
  }
  html_free(result);

  html_map_deinit(&m);
  dmt_dump(stdout);

  printf("ok\n");
  return 0;
}