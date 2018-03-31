#ifndef HTML_PARSE_H
#define HTML_PARSE_H

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
 * Parse html data and create a tree.
 * @param  workspace [current workspace]
 * @param  data      [html string]
 * @param  data_size [size of html string]
 * @return           [tree index]
 */
int html_parse_tree(html_workspace_t *workspace, const char *data, size_t data_size);

#endif