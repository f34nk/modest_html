#include "html_workspace.h"

html_workspace_t* html_init()
{
  size_t workspace_size = sizeof(struct html_workspace);

#ifdef MODEST_HTML_DEBUG
  printf("html_init() - Size of workspace is %d\n", (int)workspace_size);
#endif

  html_workspace_t* workspace = NULL;
  workspace = (html_workspace_t*)html_malloc(workspace_size);
  if(workspace == NULL) {
    fprintf(stderr, "html_init() - Failed to allocate workspace\n");
    return NULL;
  }

  // init myhtml
  myhtml_t* myhtml = myhtml_create();
  mystatus_t status = myhtml_init(myhtml, MyHTML_OPTIONS_DEFAULT, 1, 0);
  if(status != MyCORE_STATUS_OK) {
    fprintf(stderr, "html_init() - Failed to init myhtml\n");
    html_destroy(workspace);
    return NULL;
  }

  // init mycss
  mycss_t* mycss = mycss_create();
  status = mycss_init(mycss);
  if(status != MyCORE_STATUS_OK) {
    fprintf(stderr, "html_init() - Failed to init mycss\n");
    html_destroy(workspace);
    return NULL;
  }

  // init finder
  modest_finder_t* finder = modest_finder_create_simple();

  // update pointers
  workspace->myhtml = myhtml;
  workspace->mycss = mycss;
  workspace->finder = finder;

  // init arrays
  html_vec_init(&workspace->trees);
  html_vec_init(&workspace->entries);
  html_vec_init(&workspace->selectors);
  html_vec_init(&workspace->collections);
  html_vec_init(&workspace->buffers);

#ifdef MODEST_HTML_USE_DMT
  printf("html_init() - Current memory usage: %u bytes\n", (unsigned int)dmt_usage());
#endif

  return workspace;
}

void html_destroy(html_workspace_t* workspace)
{
  if(workspace == NULL) {
#ifdef MODEST_HTML_DEBUG
    fprintf(stderr, "html_destroy() - Empty workspace.\n");
#endif
    return;
  }

#ifdef MODEST_HTML_USE_DMT
  printf("html_destroy() - Current memory usage: %u bytes\n", (unsigned int)dmt_usage());
#endif

#ifdef MODEST_HTML_DEBUG
  printf("html_destroy()\n");
  printf("\tfree %d collections...\n", workspace->collections.length);
  printf("\tfree %d css entries...\n", workspace->entries.length);
  printf("\tfree %d selectors...\n", workspace->selectors.length);
  printf("\tfree %d trees...\n", workspace->trees.length);
  printf("\tfree %d buffers...\n", workspace->buffers.length);
#endif

  for (int i = 0; i < workspace->collections.length; ++i) {
    myhtml_collection_t* collection = workspace->collections.data[i];
    myhtml_collection_destroy(collection);
  }
  html_vec_deinit(&workspace->collections);

  for (int i = 0; i < workspace->entries.length; ++i) {
    mycss_entry_t* entry = workspace->entries.data[i];
    mycss_selectors_list_t* list = workspace->selectors.data[i];

    mycss_selectors_list_destroy(mycss_entry_selectors(entry), list, true);
    mycss_entry_destroy(entry, true);

  }
  html_vec_deinit(&workspace->entries);
  html_vec_deinit(&workspace->selectors);

  for (int i = 0; i < workspace->buffers.length; ++i) {
    html_vec_str_t vec = workspace->buffers.data[i];
    while(vec.length > 0) {
      char* buffer = html_vec_pop(&vec);
      html_free(buffer);
    }
    html_vec_deinit(&vec);
  }
  html_vec_deinit(&workspace->buffers);

  // destroy Modest Finder
  modest_finder_destroy(workspace->finder, true);

  // destroy MyCSS
  mycss_destroy(workspace->mycss, true);

  for (int i = 0; i < workspace->trees.length; ++i) {
    myhtml_tree_t* tree = workspace->trees.data[i];
    myhtml_tree_destroy(tree);
    html_vec_deinit(&workspace->trees);
  }

  // destroy MyHTML
  myhtml_destroy(workspace->myhtml);

  workspace->myhtml = NULL;
  workspace->mycss = NULL;
  workspace->finder = NULL;

  html_free(workspace);
  workspace = NULL;

#ifdef MODEST_HTML_USE_DMT
  dmt_dump(stdout);
#endif
}

void* html_get_scope_node(html_workspace_t* workspace, int tree_index, const char* scope_name)
{
  if(workspace == NULL) {
#ifdef MODEST_HTML_DEBUG
    fprintf(stderr, "html_scope_node() - Empty workspace.\n");
#endif
    return NULL;
  }

  // myhtml_tree_t *tree = workspace->trees.data[tree_index];
  myhtml_tree_t* tree = (myhtml_tree_t*)html_get_tree(workspace, tree_index);
  if(tree == NULL) {
#ifdef MODEST_HTML_DEBUG
    fprintf(stderr, "html_get_scope_node() - Empty tree\n");
#endif
    return NULL;
  }

  myhtml_tree_node_t* result = NULL;

  if(strcmp(scope_name, "html") == 0) {
    result = tree->node_html;
  }
  else if(strcmp(scope_name, "head") == 0) {
    result = tree->node_head;
  }
  else if(strcmp(scope_name, "body") == 0) {
    result = tree->node_body;
  }
  else if(strcmp(scope_name, "body_children") == 0) {
    // Returns first child of body.
    // To parse other children you have to call myhtml_node_next() on this node.
    result = myhtml_node_child(tree->node_body);
  }
  else if(strcmp(scope_name, "form") == 0) {
    // TODO: What is the Use Case for this option?
    result = tree->node_form;
  }
  else {
    fprintf(stderr, "html_scope_node() - Scope '%s' does not exist.\n", scope_name);
  }

  return (myhtml_tree_node_t*)result;
}

void* html_get_tree(html_workspace_t* workspace, int tree_index)
{
  if(workspace == NULL) {
#ifdef MODEST_HTML_DEBUG
    fprintf(stderr, "html_get_tree() - Empty workspace.\n");
#endif
    return NULL;
  }

  if(tree_index == -1) {
    return NULL;
  }

  if(tree_index > workspace->trees.length - 1) {
    fprintf(stderr, "html_get_tree() - Index %d out of bounds %d.\n", tree_index, workspace->trees.length);
    return NULL;
  }

  return (myhtml_tree_t*)workspace->trees.data[tree_index];
}

void* html_get_collection(html_workspace_t* workspace, int collection_index)
{
  if(workspace == NULL) {
#ifdef MODEST_HTML_DEBUG
    fprintf(stderr, "html_get_collection() - Empty workspace.\n");
#endif
    return NULL;
  }

  if(collection_index == -1) {
    return NULL;
  }

  if(collection_index > workspace->collections.length - 1) {
    fprintf(stderr, "html_get_collection() - Index %d out of bounds %d.\n", collection_index, workspace->collections.length);
    return NULL;
  }

  return (myhtml_collection_t*)workspace->collections.data[collection_index];
}

void* html_get_node(html_workspace_t* workspace, int collection_index, int index)
{
  myhtml_collection_t* collection = html_get_collection(workspace, collection_index);
  if(collection == NULL) {
    return NULL;
  }
  return (myhtml_tree_node_t*)collection->list[index];
}

void* html_get_buffer(html_workspace_t* workspace, int buffer_index)
{
  if(workspace == NULL) {
#ifdef MODEST_HTML_DEBUG
    fprintf(stderr, "html_get_buffer() - Empty workspace.\n");
#endif
    return NULL;
  }

  if(buffer_index == -1) {
    return NULL;
  }

  if(buffer_index > workspace->buffers.length - 1) {
    fprintf(stderr, "html_get_buffer() - Index %d out of bounds %d.\n", buffer_index, workspace->buffers.length);
    return NULL;
  }

  return (html_vec_str_t*)&workspace->buffers.data[buffer_index];
}