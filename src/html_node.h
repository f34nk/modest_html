#ifndef HTML_NODE_H
#define HTML_NODE_H

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

struct html_node
{
  char *tag_name;
  char *selector;
  html_vec_str_t keys;
  html_map_str_t key_value;
  char *text;
  char *parent_selector;
}
typedef html_node_t;

void html_node_init(html_node_t *params);
void html_node_destroy(html_node_t *params);
bool html_node_get(myhtml_tree_node_t* node, html_node_t *params);
void html_node_dump(FILE *file, html_node_t *params);
bool html_node_is_text(html_node_t *params);
bool html_node_has_attributes(html_node_t *params);

#endif