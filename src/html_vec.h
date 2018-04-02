#ifndef HTML_VEC_H
#define HTML_VEC_H

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

#include "vec.h"

/**
 * Vector of strings.
 * 
 */
typedef vec_t(char*) html_vec_t;
/**
 * Vector of string vectors.
 */
typedef vec_t(html_vec_t) html_vec_2d_t;

#define html_vec_foreach(v, var, iter) vec_foreach(v, var, iter)
#define html_vec_push(v, val) vec_push(v, val)
#define html_vec_pop(v) vec_pop(v)
#define html_vec_init(v) vec_init(v)
#define html_vec_deinit(v) vec_deinit(v)

#endif