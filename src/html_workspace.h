#ifndef HTML_WORKSPACE_H
#define HTML_WORKSPACE_H

/*
 Copyright (C) 2018 Frank Eickhoff

 This library is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation; either
 version 2.1 of the License, or (at your option) any later version.

 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 Lesser General Public License for more details.

 You should have received a copy of the GNU Lesser General Public
 License along with this library; if not, write to the Free Software
 Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
*/

// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <stdbool.h>

// #include <myhtml/myhtml.h>
// #include <modest/finder/finder.h>
// #include <myhtml/serialization.h>
// #include <mycss/mycss.h>
// #include <mycss/selectors/init.h>
// #include <mycss/selectors/serialization.h>

// #include "vec.h"
// #include "vec_utils.h"

// #define MODEST_HTML_DEBUG
// #define MODEST_HTML_USE_DMT

// #ifdef MODEST_HTML_USE_DMT

// #include "dmt.h"

// #define html_malloc(size) dmt_malloc(size)
// #define html_realloc(data, size) dmt_realloc(data, size)
// #define html_calloc(num, size) dmt_calloc(num, size)
// #define html_free(data) dmt_free(data)

// #else

// #define html_malloc(size) malloc(size)
// #define html_realloc(data, size) realloc(data, size)
// #define html_calloc(num, size) calloc(num, size)
// #define html_free(data) free(data)

// #endif

#include "html_includes.h"

/**
 * Vector of string vectors.
 */
typedef vec_t(vec_str_t) vec_str_vec_t;
/**
 * Vector of myhtml tree pointers.
 */
typedef vec_t(myhtml_tree_t*) tree_vec_t;
/**
 * Vector of mycss entry pointers.
 */
typedef vec_t(mycss_entry_t*) entry_vec_t;
/**
 * Vector of mycss selector list pointers.
 */
typedef vec_t(mycss_selectors_list_t*) selectors_list_vec_t;
/**
 * Vector of myhtml collection pointers.
 */
typedef vec_t(myhtml_collection_t*) collection_vec_t;
/**
 * The space we work in.
 */
struct html_workspace
{
  myhtml_t *myhtml;
  mycss_t *mycss;
  modest_finder_t *finder;
  tree_vec_t trees;
  entry_vec_t entries;
  selectors_list_vec_t selectors;
  collection_vec_t collections;
  vec_str_vec_t buffers;
}
typedef html_workspace_t;
/**
 * Initalize a workspace.
 * @return [allocated workspace]
 */
html_workspace_t* html_init();
/**
 * Destroy a workspace
 * @param workspace [current workspace]
 */
void html_destroy(html_workspace_t *workspace);
/**
 * Get tree node for scope name.
 * Scope names can be: html, head, body, body_first_child and form
 * @param workspace  [current workspace]
 * @param tree_index [target tree index]
 * @param scope_name [myhtml_tree_node_t* or NULL]
 */
void* html_get_scope_node(html_workspace_t *workspace, int tree_index, const char *scope_name);
/**
 * Get pointer to myhtml tree for index.
 * @param workspace        [current workspace]
 * @param collection_index [target tree index]
 * @return                 [myhtml_tree_t* or NULL]
 */
void* html_get_tree(html_workspace_t *workspace, int tree_index);
/**
 * Get pointer to myhtml collection for index.
 * @param workspace        [current workspace]
 * @param collection_index [target collection index]
 * @return                 [myhtml_collection_t* or NULL]
 */
void* html_get_collection(html_workspace_t *workspace, int collection_index);
/**
 * Get pointer to string vector for index.
 * @param  workspace    [current workspace]
 * @param  buffer_index [target buffer index]
 * @return              [vec_str_t* or NULL]
 */
void* html_get_buffer(html_workspace_t *workspace, int buffer_index);

#endif