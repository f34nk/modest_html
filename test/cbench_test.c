#include <stdbool.h>

#include "hires-timer.h"
#include "cbench.h"

#include "modest_html.h"
#include "test_utils.h"

// const char* selector = "*";
// const char* scope_name = "body";
// html_workspace_t* w = NULL;
// int tree_index = -1;
// int selector_index = -1;
// int collection_index = -1;
// int buffer_index = -1;
const char* filename = "../test/fixtures/wikipedia_hyperlink.html";
// const char* filename = "../test/fixtures/w3c_html5.html";
// const char* filename = "../test/fixtures/github_trending_js.html";
// const char* filename = "../test/fixtures/small.html";
char* html = NULL;

// bool hires_interval_gteq(hires_interval* a, hires_interval* b)
// {
//   if (a->secs > b->secs) {
//     return true;
//   }
//   else if (a->secs == b->secs) {
//     return a->nanosecs >= b->nanosecs;
//   }
//   else {
//     return false;
//   }
// }

// void slow_function(hires_interval* ivl)
// {
//   HIRES_TIME t_start, t_cur;
//   hires_interval cur_ivl;
//   GET_HIRES_TIME(&t_start);
//   while (1) {
//     GET_HIRES_TIME(&t_cur);
//     hires_time_diff(&t_cur, &t_start, &cur_ivl);
//     if (hires_interval_gteq(&cur_ivl, ivl)) {
//       break;
//     }
//   }
// }

// CBENCH_DEF(bench_sleep)
// {
//   int i = *(int*)CBENCH_args;

//   hires_interval ivl;
//   ivl.secs = 0;
//   ivl.nanosecs = i;

//   CBENCH_INIT("Running for %i ns", i) {
//     CBENCH_MEASURE_START
//     slow_function(&ivl);
//     CBENCH_MEASURE_STOP
//   }
// }

void parse_tree(hires_interval* ivl)
{
  html_workspace_t* w = html_init();
  int tree_index = html_parse_tree(w, html, strlen(html));
  html_destroy(w);
}

// void prepare_selector(hires_interval* ivl)
// {
//   selector_index = html_prepare_selector(w, selector, strlen(selector));
// }

// void select_nodes(hires_interval* ivl)
// {
//   collection_index = html_select(w, tree_index, scope_name, selector_index);
// }

// void serialize_collection(hires_interval* ivl)
// {
//   buffer_index = html_serialize_collection(w, collection_index);
// }

// void serialize_tree(hires_interval* ivl)
// {
//   buffer_index = html_serialize_tree(w, tree_index, scope_name);
// }

void find(hires_interval* ivl)
{
  html_workspace_t* w = html_init();
  int tree_index = html_parse_tree(w, html, strlen(html));
  const char* selector = "*";
  int selector_index = html_prepare_selector(w, selector, strlen(selector));
  const char* scope_name = "body";
  int collection_index  = html_select(w, tree_index, scope_name, selector_index);
  int buffer_index = html_serialize_collection(w, collection_index);
  html_vec_str_t* buffer = html_get_buffer(w, buffer_index);

  int i;
  char* val;
  html_vec_foreach(buffer, val, i) {
    // do something ...
  }
  html_destroy(w);
}

void serialize_tree_buffer(hires_interval* ivl)
{
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

  // myhtml_tree_node_t* scope_node = myhtml_tree_get_document(tree);
  myhtml_tree_node_t* scope_node = myhtml_tree_get_node_body(tree);
  if(myhtml_serialization_tree_buffer(scope_node, &str_raw)) {
    fprintf(stderr, "Could not serialization for the tree\n");
    exit(EXIT_FAILURE);
  }

  // printf("%s", str_raw.data);
  mycore_string_raw_destroy(&str_raw, false);

  // release resources
  myhtml_tree_destroy(tree);
  myhtml_destroy(myhtml);
}

void vec_join(hires_interval* ivl)
{
  html_vec_str_t v;

  html_vec_init(&v);
  html_vec_push(&v, "H");
  html_vec_push(&v, "e");
  html_vec_push(&v, "l");
  html_vec_push(&v, "l");
  html_vec_push(&v, "o");

  const char* delimiter = "|";
  char* result = html_vec_str_join(&v, delimiter);

  html_free(result);
  html_vec_deinit(&v);
}

CBENCH_DEF(bench_parse_tree)
{
  printf("\nbench_parse_tree\n\n");
  int i = *(int*)CBENCH_args;

  hires_interval ivl;
  ivl.secs = 0;
  ivl.nanosecs = i;

  CBENCH_INIT("Running for %i ns", i) {
    CBENCH_MEASURE_START
    parse_tree(&ivl);
    CBENCH_MEASURE_STOP
  }
}

CBENCH_DEF(bench_prepare_selector)
{
  printf("\nbench_prepare_selector\n\n");
  int i = *(int*)CBENCH_args;

  hires_interval ivl;
  ivl.secs = 0;
  ivl.nanosecs = i;

  CBENCH_INIT("Running for %i ns", i) {
    CBENCH_MEASURE_START
    prepare_selector(&ivl);
    CBENCH_MEASURE_STOP
  }
}

CBENCH_DEF(bench_select)
{
  printf("\nbench_select\n\n");
  int i = *(int*)CBENCH_args;

  hires_interval ivl;
  ivl.secs = 0;
  ivl.nanosecs = i;

  CBENCH_INIT("Running for %i ns", i) {
    CBENCH_MEASURE_START
    select_nodes(&ivl);
    CBENCH_MEASURE_STOP
  }
}

CBENCH_DEF(bench_serialize_collection)
{
  printf("\nbench_serialize_collection\n\n");
  int i = *(int*)CBENCH_args;

  hires_interval ivl;
  ivl.secs = 0;
  ivl.nanosecs = i;

  CBENCH_INIT("Running for %i ns", i) {
    CBENCH_MEASURE_START
    serialize_collection(&ivl);
    CBENCH_MEASURE_STOP
  }
}

CBENCH_DEF(bench_serialize_tree)
{
  printf("\nbench_serialize_tree\n\n");
  int i = *(int*)CBENCH_args;

  hires_interval ivl;
  ivl.secs = 0;
  ivl.nanosecs = i;

  CBENCH_INIT("Running for %i ns", i) {
    CBENCH_MEASURE_START
    serialize_tree(&ivl);
    CBENCH_MEASURE_STOP
  }
}

CBENCH_DEF(bench_find)
{
  printf("\nbench_find\n\n");
  int i = *(int*)CBENCH_args;

  hires_interval ivl;
  ivl.secs = 0;
  ivl.nanosecs = i;

  CBENCH_INIT("Running for %i ns", i) {
    CBENCH_MEASURE_START
    find(&ivl);
    CBENCH_MEASURE_STOP
  }
}

CBENCH_DEF(bench_vec_join)
{
  printf("\nbench_vec_join\n\n");
  int i = *(int*)CBENCH_args;

  hires_interval ivl;
  ivl.secs = 0;
  ivl.nanosecs = i;

  CBENCH_INIT("Running for %i ns", i) {
    CBENCH_MEASURE_START
    vec_join(&ivl);
    CBENCH_MEASURE_STOP
  }
}

CBENCH_DEF(bench_serialize_tree_buffer)
{
  printf("\nbench_serialize_tree_buffer\n\n");
  int i = *(int*)CBENCH_args;

  hires_interval ivl;
  ivl.secs = 0;
  ivl.nanosecs = i;

  CBENCH_INIT("Running for %i ns", i) {
    CBENCH_MEASURE_START
    serialize_tree_buffer(&ivl);
    CBENCH_MEASURE_STOP
  }
}

int main(int argc, char* argv[])
{
  // w = ...
  
  html = read_file(filename);
  if(html == NULL) {
    return 1;
  }

  // int cnt = 7000000; // 7 micro
  int cnt = 1000000000; // 1 sec
  INIT_TIMER();
  
  // do_bench_print_summary(CBENCH_bench_parse_tree, 10, 100, &cnt);

  // do_bench_print_summary(CBENCH_bench_prepare_selector, samples, max_runs, &cnt);

  // do_bench_print_summary(CBENCH_bench_select, samples, max_runs, &cnt);

  // do_bench_print_summary(CBENCH_bench_serialize_collection, samples, min_runs, &cnt);

  // do_bench_print_summary(CBENCH_bench_serialize_tree, samples, min_runs, &cnt);

  do_bench_print_summary(CBENCH_bench_find, 3, 100, &cnt);

  // // do_bench_print_summary(CBENCH_bench_vec_join, samples, min_runs, &cnt);

  // do_bench_print_summary(CBENCH_bench_serialize_tree_buffer, 3, 100, &cnt);

  free(html);
  // html_destroy(w);
  // printf("done\n");

  return 0;
}
