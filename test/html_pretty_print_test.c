
#include "modest_html.h"

int main(int argc, char const *argv[])
{
  html_workspace_t *w = html_init();
  int i = 0;

  // test

  const char *html = "<div class=\"block\"><p>Hello</p><p>World</p><img src=\"test.png\"/></div>";
  const char *selector = "p";
  html_result_t s1 = html_parse_and_select(w, html, selector);

  bool colorize = false;
  char *result = html_pretty_print(w, s1.collection_index, colorize);

  printf("%d: %s", ++i, result);
  if(strcmp(result, "<p>\n  Hello\n</p>\n<p>\n  World\n</p>\n") != 0){
    fprintf(stderr, "Failed\n");
    html_free(result);
    html_destroy(w);
    return 1;
  }
  html_free(result);

  html_destroy(w);
  printf("ok\n");
  return 0;
}