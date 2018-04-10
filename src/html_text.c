#include "html_text.h"

char* html_get_text_from_node(myhtml_tree_node_t* node)
{
  if(node == NULL) {
    return NULL;
  }

  myhtml_tree_node_t* text_node = myhtml_node_child(node);
  if(text_node == NULL) {
    return NULL;
  }

  const char* text = myhtml_node_text(text_node, NULL);
  if(text == NULL) {
    return NULL;
  }

  char* data = (char*)html_malloc(strlen(text) + 1);
  if(data == NULL) {
    return NULL;
  }
  strcpy(data, text);
  // User must free data.
  return data;
}

bool html_set_text_for_node(myhtml_tree_node_t* node, const char* text)
{
  if(node == NULL) {
    return false;
  }

  myhtml_tree_node_t* text_node = myhtml_node_child(node);

  // TODO: inplace html_set_text_for_node

#if 1
  if(text_node) {
#ifdef MODEST_HTML_DEBUG
    printf("html_set_text_for_node() - Remove old text node.\n");
#endif
    myhtml_node_delete(text_node);
  }

#ifdef MODEST_HTML_DEBUG
  printf("html_set_text_for_node() - Insert new text node.\n");
#endif
  myhtml_tree_node_t* new_text_node = myhtml_node_create(node->tree, MyHTML_TAG__TEXT, MyHTML_NAMESPACE_HTML);
  /*mycore_string_t *string =*/ myhtml_node_text_set(new_text_node, text, strlen(text), MyENCODING_UTF_8);

  myhtml_node_append_child(node, new_text_node);
#else
  if(text_node == NULL) {
#ifdef MODEST_HTML_DEBUG
    printf("html_set_text_for_node() - Insert new text node.\n");
#endif
    myhtml_tree_node_t* new_text_node = myhtml_node_create(node->tree, MyHTML_TAG__TEXT, MyHTML_NAMESPACE_HTML);
    /*mycore_string_t *string =*/ myhtml_node_text_set(new_text_node, text, strlen(text), MyENCODING_UTF_8);
    myhtml_node_append_child(node, new_text_node);
    return true;
  }
  else {
    mycore_string_t* string = myhtml_node_string(text_node);
    if(string == NULL) {
      return false;
    }
    // TODO: Howto realloc string data correctly?
    char* data = mycore_string_realloc(string, strlen(text));
    // char *data = mycore_realloc(string->data, strlen(text) * sizeof(char));
    if(data == NULL) {
      return false;
    }
#ifdef MODEST_HTML_DEBUG
    printf("html_set_text_for_node() - Copy new text (%d %d %d)\n", strlen(string->data), strlen(data), strlen(text));
#endif
    strcpy(data, text);
  }
#endif

  return true;
}

int html_get_text(html_workspace_t* workspace, int collection_index)
{
  if(workspace == NULL) {
#ifdef MODEST_HTML_DEBUG
    fprintf(stderr, "html_get_text() - Empty workspace.\n");
#endif
    return -1;
  }

  myhtml_collection_t* collection = html_get_collection(workspace, collection_index);
  if(collection == NULL) {
#ifdef MODEST_HTML_DEBUG
    fprintf(stderr, "html_get_text() - Empty collection\n");
#endif
    return -1;
  }

  html_vec_str_t buffer;
  html_vec_init(&buffer);

  if(collection && collection->list && collection->length) {
    for(size_t i = 0; i < collection->length; i++) {
      myhtml_tree_node_t* node = collection->list[i];
      char* text = html_get_text_from_node(node);
      if(text == NULL) {
        continue;
      }
      size_t length = strlen(text);
      if(length > 0) {
        html_vec_push(&buffer, text);
      }
    }
  }

  html_vec_push(&workspace->buffers, buffer);

  return workspace->buffers.length - 1;
}

bool html_set_text(html_workspace_t* workspace, int collection_index, const char* text)
{
  if(workspace == NULL) {
#ifdef MODEST_HTML_DEBUG
    fprintf(stderr, "html_set_text() - Empty workspace.\n");
#endif
    return false;
  }

  myhtml_collection_t* collection = html_get_collection(workspace, collection_index);
  if(collection == NULL) {
#ifdef MODEST_HTML_DEBUG
    fprintf(stderr, "html_set_text() - Empty collection\n");
#endif
    return false;
  }

  if(collection && collection->list && collection->length) {
    for(size_t i = 0; i < collection->length; i++) {
      myhtml_tree_node_t* node = collection->list[i];
      html_set_text_for_node(node, text);
    }
  }

  return true;
}