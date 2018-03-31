#ifndef HTML_PARSE_AND_SELECT_H
#define HTML_PARSE_AND_SELECT_H

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

#include "modest_html.h"

struct html_result
{
  int tree_index;
  int selector_index;
  int collection_index;
}
typedef html_result_t;

html_result_t html_parse_and_select(html_workspace_t *workspace, const char *html, const char *selector);

#endif