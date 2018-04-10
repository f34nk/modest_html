#include "modest_html.h"

int main(int argc, char const* argv[])
{
  int i = 0;

  const char* data = "Hello World";
  char* result;
  html_string_copy(data, result);
  printf("-> %s\n", result);
  if(strcmp(result, "Hello World") != 0) {
    html_free(result);
    MODEST_HTML_LOG_ERROR
    return 1;
  }
  html_free(result);

  result = NULL;
  html_string_asprintf(&result, "Mama %s is equal %d.", "John", 58);
  printf("-> %s\n", result);
  if(strcmp(result, "Mama John is equal 58.") != 0) {
    html_free(result);
    MODEST_HTML_LOG_ERROR
    return 1;
  }
  html_free(result);

#ifdef MODEST_HTML_USE_DMT
  dmt_dump(stdout);
#endif

  printf("done\n");
  return 0;
}