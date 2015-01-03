/*****************************************************************************
 *
 * Copyright (C) 2014 - 2015 Lukas Singer
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
 * Source code can be found under: <https://code.google.com/p/project-qpl/>.
 *
 ****************************************************************************/

#ifndef __MAP_H__
#define __MAP_H__

typedef struct key_value_s {
  size_t hash;
  char* key;
  void* value;
} key_value_t;

typedef struct map_s {
  size_t size;  /* maximum count */
  size_t count; /* element count */
  size_t index; /* iter index ( see iter_first() and iter_next() ) */
  key_value_t* items;
} map_t;


map_t* create_map(size_t size);

/* return NULL  -> added new value.
 * return !NULL -> updated value old value returned
 */
void* add_value(map_t* map, const char* key, void* value);

void* delete_value(map_t* map, const char* key);

void* get_value(map_t* map, const char* key);

/* returns the first element (not the first one added!)
 * returns NULL if map is empty.
 */
void* iter_first(map_t* map);

/* returns the next element
 * returns NULL if called after last element
 */
void* iter_next(map_t* map);

/* if param != 0 -> call free() on every value 
*/
void free_map(map_t* map, int free_values);

#endif /* __MAP_H__ */
