#include "html_parse.h"

int html_parse_tree(html_workspace_t *workspace, const char *data, size_t data_size)
{
  if(workspace == NULL) {
    fprintf(stderr, "html_parse_tree() - Empty workspace.\n");
    return -1;
  }

  myhtml_t *myhtml = workspace->myhtml;
  myhtml_tree_t* tree = myhtml_tree_create();
  mystatus_t status = myhtml_tree_init(tree, myhtml);
  if(status != MyCORE_STATUS_OK) {
    fprintf(stderr, "html_parse_tree() - Failed to init tree\n");
    return -1;
  }
  
  status = myhtml_parse(tree, MyENCODING_UTF_8, data, data_size);
  if(status != MyCORE_STATUS_OK) {
    fprintf(stderr, "html_parse_tree() - Failed to parse data\n");
    return -1;
  }
  html_vec_push(&workspace->trees, tree);

  return workspace->trees.length - 1;
}