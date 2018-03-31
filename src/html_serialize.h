#ifndef HTML_SERIALIZE_H
#define HTML_SERIALIZE_H

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

#include "html_includes.h"

/**
 * Serialize a collection.
 * @param  workspace        [current workspace]
 * @param  collection_index [target collection index]
 * @param  delimiter        [delimiter string]
 * @return                  [buffer index]
 */
int html_serialize_collection(html_workspace_t *workspace, int collection_index);

int html_serialize_tree(html_workspace_t *workspace, int tree_index, const char *scope_name);

char* html_vec_str_join(vec_str_t *vec, const char *delimiter);

char* html_serialize_node(myhtml_tree_node_t *node);
char* html_serialize_selector(myhtml_tree_node_t* node);

void html_dump_node(FILE *file, myhtml_tree_node_t *node);

#endif