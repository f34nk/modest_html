#include "html_pretty_print.h"

enum color_name {
  NO_COLOR = 0,
  RESTORE,
  RED,
  GREEN,
  YELLOW
}
typedef color_name_t;

/*
RESTORE=$(echo -en '\033[0m')
RED=$(echo -en '\033[00;31m')
GREEN=$(echo -en '\033[00;32m')
YELLOW=$(echo -en '\033[00;33m')
BLUE=$(echo -en '\033[00;34m')
MAGENTA=$(echo -en '\033[00;35m')
PURPLE=$(echo -en '\033[00;35m')
CYAN=$(echo -en '\033[00;36m')
LIGHTGRAY=$(echo -en '\033[00;37m')
LRED=$(echo -en '\033[01;31m')
LGREEN=$(echo -en '\033[01;32m')
LYELLOW=$(echo -en '\033[01;33m')
LBLUE=$(echo -en '\033[01;34m')
LMAGENTA=$(echo -en '\033[01;35m')
LPURPLE=$(echo -en '\033[01;35m')
LCYAN=$(echo -en '\033[01;36m')
WHITE=$(echo -en '\033[01;37m')

TODO: cleanup
 */
char* color_value[] = {
  "", // NO_COLOR
  "\033[0m", // RESTORE
  "\033[00;31m", // RED
  "\033[00;32m", // GREEN
  "\033[00;33m" // YELLOW
};

void print_string(const char* string, color_name_t color_name, html_vec_str_t *buffer)
{
  if(color_name == NO_COLOR) {
    // printf("%s", string);
    char *copy = NULL;
    html_string_copy(string, copy);
    html_vec_push(buffer, copy);
  }
  else {
    char *copy = NULL;
    html_string_copy(color_value[color_name], copy);
    html_vec_push(buffer, copy);

    copy = NULL;
    html_string_copy(string, copy);
    html_vec_push(buffer, copy);

    copy = NULL;
    html_string_copy(color_value[RESTORE], copy);
    html_vec_push(buffer, copy);
  }
}

void print_node_attr(myhtml_tree_node_t *node, bool colorize, html_vec_str_t *buffer)
{
  myhtml_tree_attr_t *attr = myhtml_node_attribute_first(node);
  while(attr) {
    const char *key = myhtml_attribute_key(attr, NULL);
    if(key) {
      // printf(" %s", key);
      print_string(" ", (colorize) ? GREEN : NO_COLOR, buffer);
      print_string(key, (colorize) ? GREEN : NO_COLOR, buffer);
      print_string("=", (colorize) ? GREEN : NO_COLOR, buffer);
      const char *value = myhtml_attribute_value(attr, NULL);
      if(value) {
        // printf("\"%s\"", value);
        print_string("\"", NO_COLOR, buffer);
        print_string(value, (colorize) ? YELLOW : NO_COLOR, buffer);
        print_string("\"", NO_COLOR, buffer);
      }
    }
    attr = myhtml_attribute_next(attr);
  }
}

void print_node(myhtml_tree_node_t *node, size_t indent, bool colorize, html_vec_str_t *buffer)
{
  if(node == NULL) {
    return;
  }

  // while(node && indent == 0) {

    for(size_t i = 0; i < indent; i++) {
      // printf("  ");
      print_string("  ", NO_COLOR, buffer);
    }

    myhtml_tree_t* tree = node->tree;
    const char *tag_name = myhtml_tag_name_by_id(tree, myhtml_node_tag_id(node), NULL);
    if(strcmp(tag_name, "-text") == 0) {
      myhtml_tag_id_t tag_id = myhtml_node_tag_id(node);
      if(tag_id == MyHTML_TAG__TEXT || tag_id == MyHTML_TAG__COMMENT) {
        const char* node_text = myhtml_node_text(node, NULL);
        // printf("%s", node_text);
        print_string(node_text, NO_COLOR, buffer);
      }
    }
    else {
      // printf("<%s", tag_name);
      print_string("<", NO_COLOR, buffer);
      print_string(tag_name, (colorize) ? RED : NO_COLOR, buffer);

      print_node_attr(node, colorize, buffer);
      if(myhtml_node_is_close_self(node)) {
        // printf(" />\n");
        print_string(" />\n", NO_COLOR, buffer);
      }
      else {
        // printf(">\n");
        print_string(">\n", NO_COLOR, buffer);
      }
    }

    print_node(myhtml_node_child(node), (indent + 1), colorize, buffer);

    if(strcmp(tag_name, "-text") == 0) {
      // printf("\n");
      print_string("\n", NO_COLOR, buffer);
    }
    else {
      if(!myhtml_node_is_close_self(node)) {
        for(size_t i = 0; i < indent; i++) {
          // printf("  ");
          print_string("  ", NO_COLOR, buffer);
        }
        // printf("</%s>\n", tag_name);
        print_string("</", NO_COLOR, buffer);
        print_string(tag_name, (colorize) ? RED : NO_COLOR, buffer);
        print_string(">\n", NO_COLOR, buffer);
      }
    }

    if(indent != 0) {
      print_node(myhtml_node_next(node), indent, colorize, buffer);
    }
//     node = myhtml_node_next(node);
//   }
}

char* html_pretty_print(html_workspace_t *workspace, int collection_index, bool colorize)
{
  if(workspace == NULL) {
    fprintf(stderr, "html_pretty_print() - Empty workspace.\n");
    return NULL;
  }

  myhtml_collection_t *collection = html_get_collection(workspace, collection_index);
  if(collection == NULL) {
    fprintf(stderr, "html_pretty_print() - Empty collection\n");
    return NULL;
  }

  if(collection && collection->list && collection->length) {

    html_vec_str_t vec;
    html_vec_init(&vec);

    for(size_t i = 0; i < collection->length; i++) {
      myhtml_tree_node_t *node = collection->list[i];
      print_node(node, 0, colorize, &vec);
    }

    char *result = html_vec_join(&vec, "");

    while(vec.length > 0) {
      char *buffer = html_vec_pop(&vec);
      html_free(buffer);
    }
    html_vec_deinit(&vec);

    return result;
  }
  return NULL;
}