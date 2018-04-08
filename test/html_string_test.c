#include "modest_html.h"

int main(int argc, char const *argv[])
{
  int i = 0;

  const char *data = "Hello World";
  char *result;
  html_string_copy(data, result);
  printf("-> %s\n", result);
  if(strcmp(result, "Hello World") != 0){
    fprintf(stderr, "Failed\n");
    html_free(result);
    MODEST_HTML_ERROR
    return 1;
  }
  html_free(result);

  result = NULL;
  html_string_asprintf(&result, "Mama %s is equal %d.", "John", 58);
  printf("-> %s\n", result);
  if(strcmp(result, "Mama John is equal 58.") != 0){
    fprintf(stderr, "Failed\n");
    html_free(result);
    MODEST_HTML_ERROR
    return 1;
  }
  html_free(result);

#ifdef MODEST_HTML_USE_DMT
  dmt_dump(stdout);
#endif

  printf("ok\n");
  return 0;
}