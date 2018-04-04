#ifndef HTML_COMPARE_NODE_H
#define HTML_COMPARE_NODE_H

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

// void html_compare_nodes(html_workspace_t *workspace, html_node_t *node1, html_node_t *node2, html_vec_int_t *buffer_indices);
void html_compare_nodes(html_workspace_t *workspace, myhtml_tree_node_t *node1, myhtml_tree_node_t *node2, int indent, html_vec_int_t *buffer_indices);

#endif