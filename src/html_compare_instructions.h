#ifndef HTML_COMPARE_INSTRUCTIONS_H
#define HTML_COMPARE_INSTRUCTIONS_H

#include "html_includes.h"

void add_append_instruction(html_workspace_t* workspace, myhtml_tree_node_t* node, html_vec_int_t* buffer_indices)
{
  myhtml_tree_node_t* parent_node = myhtml_node_parent(node);

  char* selector = html_serialize_selector(parent_node);
  char* html = html_serialize_node(node);

#ifdef MODEST_HTML_DEBUG
  printf("append '%s' to '%s'\n", html, selector);
#endif

  html_vec_str_t vec;
  html_vec_init(&vec);

  char* copy;
  html_string_copy("append", copy);
  html_vec_push(&vec, copy);

  html_vec_push(&vec, selector);
  html_vec_push(&vec, html);

  html_vec_push(&workspace->buffers, vec);
  html_vec_push(buffer_indices, workspace->buffers.length - 1);
}

void add_remove_instruction(html_workspace_t* workspace, myhtml_tree_node_t* node, html_vec_int_t* buffer_indices)
{
  char* selector = html_serialize_selector(node);

#ifdef MODEST_HTML_DEBUG
  printf("remove '%s'\n", selector);
#endif

  html_vec_str_t vec;
  html_vec_init(&vec);

  char* copy;
  html_string_copy("remove", copy);
  html_vec_push(&vec, copy);

  html_vec_push(&vec, selector);

  html_vec_push(&workspace->buffers, vec);
  html_vec_push(buffer_indices, workspace->buffers.length - 1);
}

void add_set_attribute_instruction(html_workspace_t* workspace, const char* selector, const char* key, const char* value, html_vec_int_t* buffer_indices)
{
#ifdef MODEST_HTML_DEBUG
  printf("set_attribute '%s = %s' for '%s'\n", key, value, selector);
#endif

  html_vec_str_t vec;
  html_vec_init(&vec);

  char* copy;
  html_string_copy("set_attribute", copy);
  html_vec_push(&vec, copy);

  char* selector_copy;
  html_string_copy(selector, selector_copy);
  html_vec_push(&vec, selector_copy);

  char* key_copy;
  html_string_copy(key, key_copy);
  html_vec_push(&vec, key_copy);

  char* value_copy;
  html_string_copy(value, value_copy);
  html_vec_push(&vec, value_copy);

  html_vec_push(&workspace->buffers, vec);
  html_vec_push(buffer_indices, workspace->buffers.length - 1);
}

void add_set_text_instruction(html_workspace_t* workspace, const char* selector, const char* text, html_vec_int_t* buffer_indices)
{
#ifdef MODEST_HTML_DEBUG
  printf("set_text '%s' for '%s'\n", text, selector);
#endif

  html_vec_str_t vec;
  html_vec_init(&vec);

  char* copy;
  html_string_copy("set_text", copy);
  html_vec_push(&vec, copy);

  char* selector_copy;
  html_string_copy(selector, selector_copy);
  html_vec_push(&vec, selector_copy);

  char* text_copy;
  html_string_copy(text, text_copy);
  html_vec_push(&vec, text_copy);

  html_vec_push(&workspace->buffers, vec);
  html_vec_push(buffer_indices, workspace->buffers.length - 1);
}

void add_set_tag_instruction(html_workspace_t* workspace, const char* selector, const char* tag_name, html_vec_int_t* buffer_indices)
{
#ifdef MODEST_HTML_DEBUG
  printf("set_tag '%s' for '%s'\n", tag_name, selector);
#endif

  html_vec_str_t vec;
  html_vec_init(&vec);

  char* copy;
  html_string_copy("set_tag", copy);
  html_vec_push(&vec, copy);

  char* selector_copy;
  html_string_copy(selector, selector_copy);
  html_vec_push(&vec, selector_copy);

  char* tag_name_copy;
  html_string_copy(tag_name, tag_name_copy);
  html_vec_push(&vec, tag_name_copy);

  html_vec_push(&workspace->buffers, vec);
  html_vec_push(buffer_indices, workspace->buffers.length - 1);
}

#endif