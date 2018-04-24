#include <time.h>

#include "modest_html.h"
#include "test_utils.h"

/*
To check for memory leaks execute test with valgrind.

  valgrind --leak-check=yes test/clock_test
*/

#define CLOCK_LOG fprintf(stdout, "[LOG]\t%s()\n", __func__);
#define CLOCK_LOG_ERROR fprintf(stderr, "[ERROR]\t%s()\n\t%s:%d\n", __func__, __FILE__, __LINE__);

const char* filename = "../test/fixtures/wikipedia_hyperlink.html";
// const char* filename = "../test/fixtures/w3c_html5.html";
// const char* filename = "../test/fixtures/github_trending_js.html";
// const char* filename = "../test/fixtures/small.html";

int myhtml_serialization_tree_buffer_bench(int c, const char* html)
{
  if(c == 0) {
    CLOCK_LOG
  }

  // basic init
  myhtml_t* myhtml = myhtml_create();
  myhtml_init(myhtml, MyHTML_OPTIONS_DEFAULT, 1, 0);

  // init tree
  myhtml_tree_t* tree = myhtml_tree_create();
  myhtml_tree_init(tree, myhtml);

  // parse html
  myhtml_parse(tree, MyENCODING_UTF_8, html, strlen(html));

  mycore_string_raw_t str_raw;
  mycore_string_raw_clean_all(&str_raw);

  myhtml_tree_node_t* scope_node = myhtml_tree_get_document(tree);
  // myhtml_tree_node_t* scope_node = myhtml_tree_get_node_body(tree);
  if(myhtml_serialization_tree_buffer(scope_node, &str_raw)) {
    CLOCK_LOG_ERROR
    return 1;
  }

  // printf("%s", str_raw.data);
  mycore_string_raw_destroy(&str_raw, false);

  // release resources
  myhtml_tree_destroy(tree);
  myhtml_destroy(myhtml);
  return 0;
}

int parse_tree_bench(int c, const char* html)
{
  if(c == 0) {
    CLOCK_LOG
  }
  html_workspace_t* w = html_init();

  int tree_index = html_parse_tree(w, html, strlen(html));

  html_destroy(w);
  if(tree_index == -1) {
    CLOCK_LOG_ERROR
    return 1;
  }
  return 0;
}

int parse_and_select_bench(int c, const char* html)
{
  if(c == 0) {
    CLOCK_LOG
  }
  html_workspace_t* w = html_init();

  int tree_index = html_parse_tree(w, html, strlen(html));
  const char* selector = "*";
  int selector_index = html_prepare_selector(w, selector, strlen(selector));
  const char* scope_name = "html";
  int collection_index = html_select(w, tree_index, scope_name, selector_index);

  html_destroy(w);
  if(collection_index == -1) {
    CLOCK_LOG_ERROR
    return 1;
  }
  return 0;
}

int parse_select_and_serialize_collection_bench(int c, const char* html)
{
  if(c == 0) {
    CLOCK_LOG
  }
  html_workspace_t* w = html_init();

  int tree_index = html_parse_tree(w, html, strlen(html));
  const char* selector = "body > *";
  int selector_index = html_prepare_selector(w, selector, strlen(selector));
  const char* scope_name = "body";
  int collection_index = html_select(w, tree_index, scope_name, selector_index);
  int buffer_index = html_serialize_collection(w, collection_index);
  html_vec_str_t* buffer = html_get_buffer(w, buffer_index);

  int i;
  char* val;
  html_vec_foreach(buffer, val, i) {
    // do something ...
  }

  html_destroy(w);
  if(buffer_index == -1) {
    CLOCK_LOG_ERROR
    return 1;
  }
  return 0;
}

int parse_select_and_serialize_tree_bench(int c, const char* html)
{
  if(c == 0) {
    CLOCK_LOG
  }
  html_workspace_t* w = html_init();

  int tree_index = html_parse_tree(w, html, strlen(html));
  const char* selector = "*";
  int selector_index = html_prepare_selector(w, selector, strlen(selector));
  const char* scope_name = "html";
  int collection_index = html_select(w, tree_index, scope_name, selector_index);
  int buffer_index = html_serialize_tree(w, tree_index, scope_name);
  html_vec_str_t* buffer = html_get_buffer(w, buffer_index);

  int i;
  char* val;
  html_vec_foreach(buffer, val, i) {
    // do something ...
  }

  html_destroy(w);
  if(buffer_index == -1) {
    CLOCK_LOG_ERROR
    return 1;
  }
  return 0;
}

int parse_select_and_serialize_tree_buffer_bench(int c, const char* html)
{
  if(c == 0) {
    CLOCK_LOG
  }
  html_workspace_t* w = html_init();

  int tree_index = html_parse_tree(w, html, strlen(html));
  const char* selector = "*";
  int selector_index = html_prepare_selector(w, selector, strlen(selector));
  const char* scope_name = "html";
  int collection_index = html_select(w, tree_index, scope_name, selector_index);

  // myhtml

  myhtml_tree_t* tree = (myhtml_tree_t*)html_get_tree(w, tree_index);
  mycore_string_raw_t str_raw;
  mycore_string_raw_clean_all(&str_raw);

  myhtml_tree_node_t* scope_node = myhtml_tree_get_document(tree);
  // myhtml_tree_node_t* scope_node = myhtml_tree_get_node_body(tree);
  if(myhtml_serialization_tree_buffer(scope_node, &str_raw)) {
    CLOCK_LOG_ERROR
    return 1;
  }

  // printf("%s", str_raw.data);
  mycore_string_raw_destroy(&str_raw, false);


  html_destroy(w);
  return 0;
}

#define clock_to_sec(t) ((double)t)/CLOCKS_PER_SEC
#define clock_to_millisec(t) (((double)t)/CLOCKS_PER_SEC) * 1000.0

#define max_benchmarks 6
int (*bench[max_benchmarks])() = {myhtml_serialization_tree_buffer_bench, parse_tree_bench, parse_and_select_bench, parse_select_and_serialize_collection_bench, parse_select_and_serialize_tree_bench, parse_select_and_serialize_tree_buffer_bench};

int main(int argc, char* argv[])
{
  char* html = read_file(filename);
  if(html == NULL) {
    return 1;
  }

  // parse_select_and_serialize_tree1_bench(0, html);
  // return 0;

  int i = 0;
  int result = 0;
  while(i < max_benchmarks && result == 0) {
    clock_t start = clock();
    clock_t diff = clock() - start;
    int c = 0;
    while(clock_to_sec(diff) < 1.0 && result == 0) {
      result = bench[i](c, html);
      diff = clock() - start;
      c += 1;
    }
    clock_t end = clock() - start;
    double average = clock_to_millisec(end) / c;
    printf("\trun %d times\ttotal %2.4f sec\taverage = %2.4f millisec\n", c, (float)clock_to_sec(end), average);
    i += 1;
  }

  free(html);
  return 0;
}