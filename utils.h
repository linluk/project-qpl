/*****************************************************************************
 *
 * Copyright (C) 2014 - 2016 Lukas Singer
 *
 * This file is part of 'qpl'.
 *
 * 'qpl' is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * 'qpl' is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with 'qpl'.  If not, see <http://www.gnu.org/licenses/>.
 *
 * The name 'qpl' stands for Quick Programming Language and is a working
 * title.  It may changes in the future.
 *
 * Source code can be found under: <https://github.com/linluk/project-qpl/>.
 *
 ****************************************************************************/

#ifndef __UTILS_H__
#define __UTILS_H__

#include <stdlib.h>

/* if allocation fails program terminates! */
void* check_malloc(size_t size);
void* check_realloc(void* ptr, size_t size);

/* sets the pointer to NULL and frees the allocated value */
void free_null(void** p);

char* replace_str(const char* str, const char* old, const char* new);

char is_str_int(const char* str);
char is_str_dbl(const char* str);

#endif /* __UTILS_H__ */

