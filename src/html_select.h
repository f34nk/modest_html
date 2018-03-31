#ifndef HTML_SELECT_H
#define HTML_SELECT_H

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
 * Prepare a CSS selector from pattern string.
 * @param  workspace     [current workspace]
 * @param  selector      [CSS pattern string]
 * @param  selector_size [size of CSS pattern string]
 * @return               [selector index]
 */
int html_prepare_selector(html_workspace_t *workspace, const char *selector, size_t selector_size);
/**
 * Execute a selector pattern on a tree.
 * @param  workspace      [current workspace]
 * @param  tree_index     [target tree index]
 * @param  scope_name     [target scope name]
 * @param  selector_index [target selector index]
 * @return                [collection index]
 */
int html_select(html_workspace_t *workspace, int tree_index, const char *scope_name, int selector_index);

#endif