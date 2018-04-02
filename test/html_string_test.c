#include "modest_html.h"

int main(int argc, char const *argv[])
{
  int i = 0;
  const char *data = "Hello World";
  char *result = html_string_copy(data);
  printf("%d: %s\n", ++i, result);
  if(strcmp(result, "Hello World") != 0){
    fprintf(stderr, "Failed\n");
    html_free(result);
    return 1;
  }
  html_free(result);

#ifdef MODEST_HTML_USE_DMT
  dmt_dump(stdout);
#endif
  
  return 0;
}