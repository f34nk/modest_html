#include "modest_html.h"
/*
To check for memory leaks execute test with valgrind.

  valgrind --leak-check=yes test/html_vec_join_test

*/
int main(int argc, char const *argv[])
{
  int i = 0;
  vec_str_t v;

  vec_init(&v);
  vec_push(&v, "H");
  vec_push(&v, "e");
  vec_push(&v, "l");
  vec_push(&v, "l");
  vec_push(&v, "o");

  const char *delimiter = "|";
  char *result = html_vec_str_join(&v, delimiter);
  printf("%d: %s\n", ++i, result);
  if(strcmp(result, "H|e|l|l|o") != 0){
    fprintf(stderr, "Failed\n");
    html_free(result);
    vec_deinit(&v);
    return 1;
  }
  html_free(result);

  delimiter = "";
  result = html_vec_str_join(&v, delimiter);
  printf("%d: %s\n", ++i, result);
  if(strcmp(result, "Hello") != 0){
    fprintf(stderr, "Failed\n");
    html_free(result);
    vec_deinit(&v);
    return 1;
  }

  html_free(result);
  vec_deinit(&v);

  dmt_dump(stdout);

  printf("ok\n");
  return 0;
}