#include "html_compare.h"

// concat str1 and str2
char *concat_string(const char *str1, const char *str2)
{
  char *result = NULL;
  size_t n = 0;

  if(str1) n += strlen(str1);
  if(str2) n += strlen(str2);

  if((str1 || str2) && (result = html_malloc(n + 1)) != NULL)
  {
    *result = '\0';

    if(str1) strcpy(result, str1);
    if(str2) strcat(result, str2);
  }

  return result;
}

/**
 * https://stackoverflow.com/a/9210560
 * @param  string   [description]
 * @param  delimiter [description]
 * @return         [description]
 */
char** split_string(char* string, const char delimiter)
{
  char** result    = 0;
  size_t count     = 0;
  char* tmp        = string;
  char* last_comma = 0;
  char delim[2];
  delim[0] = delimiter;
  delim[1] = 0;

  /* Count how many elements will be extracted. */
  while (*tmp)
  {
    if (delimiter == *tmp)
    {
      count++;
      last_comma = tmp;
    }
    tmp++;
  }

  /* Add space for trailing token. */
  count += last_comma < (string + strlen(string) - 1);

  /* Add space for terminating null string so caller
     knows where the list of returned strings ends. */
  count++;

  result = html_malloc(sizeof(char*) * count);

  if (result)
  {
    size_t idx  = 0;
    char* token = strtok(string, delim);

    while (token)
    {
      // assert(idx < count);
      *(result + idx++) = strdup(token);
      token = strtok(0, delim);
    }
    // assert(idx == count - 1);
    *(result + idx) = 0;
  }

  return result;
}

bool is_text_node(myhtml_tree_node_t* node)
{
  if(node){
    const char *tag_name = myhtml_tag_name_by_id(node->tree, myhtml_node_tag_id(node), NULL);
    if(strcmp(tag_name, "-text") == 0){
      return true;
    }
  }
  return false;
}

bool has_attributes(myhtml_tree_node_t* node)
{
  if(node){
    myhtml_tree_attr_t *attr = myhtml_node_attribute_first(node);
    return (attr) ? true : false;
  }
  return false;
}

// char* remove_scope_from_selector(char* selector, const char* scope){
//   if(strcmp(scope, "body_children") == 0){
//     remove_substring(selector, "html ");
//     remove_substring(selector, "body ");
//   }
//   return selector;
// }

bool compare_tag_names(myhtml_tree_node_t* node1, myhtml_tree_node_t* node2)
{
  if(node1 && node2){
    const char *tag_name1 = myhtml_tag_name_by_id(node1->tree, myhtml_node_tag_id(node1), NULL);
    const char *tag_name2 = myhtml_tag_name_by_id(node2->tree, myhtml_node_tag_id(node2), NULL);
    if(strcmp(tag_name1, tag_name2) == 0){
      return true;
    }
  }
  return false;
}

bool compare_text(myhtml_tree_node_t* node1, myhtml_tree_node_t* node2)
{
  if(node1 && node2){
    mycore_string_t *string1 = myhtml_node_string(node1);
    mycore_string_t *string2 = myhtml_node_string(node2);
    char *data1 = mycore_string_data(string1);
    char *data2 = mycore_string_data(string2);
    if(strcmp(data1, data2) == 0){
      return true;
    }
  }
  return false;
}

vec_str_t get_attributes(myhtml_tree_node_t* node)
{
  vec_str_t v;
  vec_init(&v);

  if(node) {
    myhtml_tree_attr_t *attr = myhtml_node_attribute_first(node);
    if(attr){

      while (attr) {
        char *name = myhtml_attribute_key(attr, NULL);
        if(name) {
          // printf("%s = ", name);
          char *value = myhtml_attribute_value(attr, NULL);
          if(value){
            // printf("%s", value);
            char *buf = concat_string(name, "=");
            buf = concat_string(buf, value);
            // printf("%s\n", buf);
            vec_push(&v, buf);
          }
          // printf("\n");
        }
        attr = myhtml_attribute_next(attr);
      } // while attr

    } // if attr
  }
  return v;
}

bool compare_attributes(myhtml_tree_node_t* node1, myhtml_tree_node_t* node2, vec_str_vec_t *instructions)
{
  vec_str_t v1 = get_attributes(node1);
  vec_str_t v2 = get_attributes(node2);
  // char *selector = serialize_selector(node1);
  // remove_scope_from_selector(selector, scope);

  bool flag = true;
  if(v1.length == 0 && v2.length == 0){
    flag = true;
  }
  else if(v1.length == v2.length){
    for(int i = 0; i < v1.length; i++){
      char *val1 = v1.data[i];
      char *val2 = v2.data[i];
      if(strcmp(val1, val2) != 0){
        char** kv = split_string(val2, '=');
#ifdef MODEST_HTML_DEBUG
        printf("set_attribute '%s' to '%s'\n", val2, "selector");
#endif
        // add_set_attribute_instruction(result, selector, kv[0], kv[1]);
        free(kv);
        flag = false;
      }
    }
  }
  else if(v1.length > v2.length){
    for(int i = 0; i < v1.length; i++){
      if(i >= v2.length){
        char *val1 = v1.data[i];
        char** kv = split_string(val1, '=');
#ifdef MODEST_HTML_DEBUG
        printf("set_attribute '%s = NULL' to '%s'\n", kv[0], "selector");
#endif
        // add_set_attribute_instruction(result, selector, kv[0], "");
        free(kv);
        flag = false;
      }
    }
  }
  else if(v1.length < v2.length){
    for(int i = 0; i < v2.length; i++){
      if(i >= v1.length){
        char *val2 = v2.data[i];
        char** kv = split_string(val2, '=');
#ifdef MODEST_HTML_DEBUG
        printf("set_attribute '%s' to '%s'\n", val2, "selector");
#endif
        // add_set_attribute_instruction(result, selector, kv[0], kv[1]);
        free(kv);
        flag = false;
      }
    }
  }
  vec_deinit(&v1);
  vec_deinit(&v2);
  return flag;
}

void compare_nodes(myhtml_tree_node_t* node1, myhtml_tree_node_t* node2, int indent, vec_str_vec_t *instructions)
{
#ifdef MODEST_HTML_DEBUG
    for(int i = 0; i < indent; i++){
      printf(".");
    }
#endif

  if(!node1 && node2) {
    // is missing in html1
    // append to html1
    // myhtml_tree_node_t* parent_node2 = myhtml_node_parent(node2);

    // char *selector2 = serialize_selector(parent_node2);
    // remove_scope_from_selector(selector2, scope);
    char *html = html_serialize_node(node2);

#ifdef MODEST_HTML_DEBUG
    printf("append '%s' to '%s'\n", html, "selector2");
#endif
    // add_append_instruction(result, selector2, html);

    // html_free(selector2);
    html_free(html);
  }
  else if(node1 && !node2) {
    // is missing in html2
    // remove from html1
    // myhtml_tree_node_t* parent_node1 = myhtml_node_parent(node1);
    // const char *parent_tag_name1 = myhtml_tag_name_by_id(node1->tree, myhtml_node_tag_id(parent_node1), NULL);
    // printf("remove '%s' from '%s' in html1\n", tag_name1, parent_tag_name1);

    // char *selector1 = serialize_selector(node1);
    // remove_scope_from_selector(selector1, scope);
    char *html = html_serialize_node(node1);

#ifdef MODEST_HTML_DEBUG
    printf("remove '%s' at '%s'\n", html, "selector1");
#endif
    // add_remove_instruction(result, selector1);

    // html_free(selector1);
    html_free(html);
  }
  else if(!node1 && !node2){
#ifdef MODEST_HTML_DEBUG
    printf("NULL NULL\n");
#endif
  }
  else {
    if(compare_tag_names(node1, node2) == true){
      if(is_text_node(node1) && is_text_node(node2)){
        if(compare_text(node1, node2) == false){

          // myhtml_tree_node_t* parent_node1 = myhtml_node_parent(node1);
          // char *selector1 = serialize_selector(parent_node1);
          // remove_scope_from_selector(selector1, scope);

          // myhtml_tree_node_t* parent_node2 = myhtml_node_parent(node2);
          // char *selector2 = serialize_selector(parent_node2);
          // remove_scope_from_selector(selector2, scope);

          mycore_string_t *string2 = myhtml_node_string(node2);
          char *data2 = mycore_string_data(string2);
#ifdef MODEST_HTML_DEBUG
          printf("set_text '%s' to '%s'\n", data2, "selector2");
#endif
          // add_set_text_instruction(result, selector2, data2);

          // free(selector1);
          // free(selector2);
        }
      }
      else if (compare_attributes(node1, node2, instructions) == true) {
        const char *tag_name1 = myhtml_tag_name_by_id(node1->tree, myhtml_node_tag_id(node1), NULL);
        const char *tag_name2 = myhtml_tag_name_by_id(node2->tree, myhtml_node_tag_id(node2), NULL);
#ifdef MODEST_HTML_DEBUG
        printf("'%s' == '%s'\n", tag_name1, tag_name2);
#endif
      }
    }
    else {
      // char *selector1 = serialize_selector(node1);
      // remove_scope_from_selector(selector1, scope);
      // char *selector2 = serialize_selector(node2);
      // remove_scope_from_selector(selector2, scope);
      const char *tag_name2 = myhtml_tag_name_by_id(node2->tree, myhtml_node_tag_id(node2), NULL);
#ifdef MODEST_HTML_DEBUG
      printf("set_tag '%s' for '%s'\n", tag_name2, "selector1");
#endif
      // add_set_tag_instruction(result, selector1, tag_name2);

      // free(selector1);
      // free(selector2);
    }
  } // else
}

void compare_trees(myhtml_tree_node_t *node1, myhtml_tree_node_t *node2, int indent, vec_str_vec_t *instructions)
{
  myhtml_tree_node_t* child_node1 = NULL;
  myhtml_tree_node_t* child_node2 = NULL;
  myhtml_tree_node_t* next_node1 = NULL;
  myhtml_tree_node_t* next_node2 = NULL;

  if(node1){
    child_node1 = myhtml_node_child(node1);
    next_node1 = myhtml_node_next(node1);
  }
  if(node2){
    child_node2 = myhtml_node_child(node2);
    next_node2 = myhtml_node_next(node2);
  }

  compare_nodes(node1, node2, indent, instructions);

  if(child_node1 && child_node2) {
  // if(child_node1 || child_node2) {
    compare_trees(child_node1, child_node2, indent + 1, instructions);
  }

  if(next_node1 || next_node2){
    compare_trees(next_node1, next_node2, indent, instructions);
  }
}

int html_compare(html_workspace_t *workspace, int collection1_index, int collection2_index)
{
  if(workspace == NULL) {
    fprintf(stderr, "html_compare() - Empty workspace.\n");
    return -1;
  }
  
  myhtml_collection_t *collection1 = html_get_collection(workspace, collection1_index);
  if(collection1 == NULL) {
    fprintf(stderr, "html_compare() - Empty collection1\n");
    return -1;
  }

  myhtml_collection_t *collection2 = html_get_collection(workspace, collection2_index);
  if(collection2 == NULL) {
    fprintf(stderr, "html_compare() - Empty collection2\n");
    return -1;
  }

  if(collection1 && collection1->list && collection1->length &&
     collection2 && collection2->list && collection2->length) {
    
    myhtml_tree_node_t *node1 = collection1->list[0];
    myhtml_tree_node_t *node2 = collection2->list[0];

    vec_str_vec_t instructions;
    vec_init(&instructions);

    compare_trees(node1, node2, 0, &instructions);

    return -1;
    // vec_push(&workspace->buffers, buffer);
    // return workspace->buffers.length - 1;
  }

  return -1;
}