#ifndef HTML_SLICE_H
#define HTML_SLICE_H

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
 * Reduce the set of matched elements to a subset specified by a range of indices.
 * @param  workspace        [current workspace]
 * @param  collection_index [target collection index]
 * @param  start            [start index]
 * @param  end              [end index]
 * @return                  [index of new collection]
 */
int html_slice(html_workspace_t *workspace, int collection_index, int start, int end);

#endif