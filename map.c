/*****************************************************************************
 *
 * Copyright (C) 2014 Lukas Singer
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

#include <stdlib.h>
#include <stdio.h>
#include <string.h> /* TODO: refactor hash from string functions to pointer arithmetic */

#include "utils.h"

#include "map.h"

/* defines */
#define FNV_PRIME_32 (16777619)
#define FNV_OFFSET_BASIS_32 (2166136261)

//#define MAP_PARANOID_INFINITY_LOOP_CHECK
//#define DO_MAP_DEBUG

#define MAP_THRESHOLD (5)
#define MIN_MAP_SIZE (15)

/* prototypes */
size_t get_hash(const char* key);

void error_exit(char* emsg);

int is_empty_item(key_value_t item);

void rehash(map_t* map, size_t new_size);  /* TODO: rename to repopulate */

#ifdef DO_MAP_DEBUG
void debug_print_map(map_t* map, const char* msg);
#endif /* DO_MAP_DEBUG */

/* prototypes in "map.c" */
size_t get_hash(const char* key) {
  /* implements fnv-1 hashing function */
  size_t hash;
  size_t len;
  size_t idx;
  hash = FNV_OFFSET_BASIS_32;
  len = strlen(key);
  idx = 0;
  while (idx < len) {
    hash *= FNV_PRIME_32;
    hash ^= key[idx];
    idx++;
  }
  return hash;
}

#ifdef DO_MAP_DEBUG
void debug_print_map(map_t* map, const char* msg) {
  size_t i;
  printf("*** MAP DEBUG PRINT ***\n(%s)\n",msg);
  for(i = 0; i < map->size; i++) {
    if(is_empty_item(map->items[i])) {
      printf("element on position %zu is empty\n",i);
    }else{
      printf("element (%s) %zu has hash %zu, want to be element %zu\n",map->items[i].key,i,map->items[i].hash,map->items[i].hash % map->size);
    }
  }
  printf("map size: %zu\nmap count: %zu\n",map->size,map->count);
}
#endif /* DO_MAP_DEBUG */

void error_exit(char* emsg) {
  fprintf(stderr, "error: \"%s\"", emsg);
  exit(1);
}

int is_empty_item(key_value_t item) {
//  return (item.key == NULL && item.hash == 0 && item.value == NULL);
  return (item.key == NULL);
}

void rehash(map_t* map, size_t new_size) {
 #ifdef DO_MAP_DEBUG
  debug_print_map(map,"before rehash()");
#endif /* DO_MAP_DEBUG */ 
  if(map->count > 0) {
    key_value_t tmp[map->count];
    size_t i1, i2;
    // extract all items
    i1 = 0;
    i2 = 0;
    while(i1 < map->size) {
      if(!is_empty_item(map->items[i1])) {
        tmp[i2].key = map->items[i1].key;
        tmp[i2].value = map->items[i1].value;
        tmp[i2].hash = map->items[i1].hash;
        i2++;
      }
      i1++;
    }
    free(map->items);
    // resize map
    i1 = map->count + MAP_THRESHOLD;
    i2 = i1 > MIN_MAP_SIZE ? i1 : MIN_MAP_SIZE;
    map->size = new_size > i2 ? new_size : i2;
    map->items = (key_value_t*)check_malloc(sizeof(key_value_t) * map->size);
    for(i1 = 0; i1 < map->size; i1++) {
      map->items[i1].key = NULL;
      map->items[i1].value = NULL;
      map->items[i1].hash = 0;
    }
    // repopulate map
#ifdef DO_MAP_DEBUG
      printf("*** MAP DEBUG REMAP ***\n");
#endif /* DO_MAP_DEBUG */
      for(i1 = 0; i1 < map->count; i1++) {
      i2 = tmp[i1].hash % map->size;
      while(!is_empty_item(map->items[i2])) {
        i2++;
        if(i2 >= map->size) {
          i2 = 0;
        }
      }
#ifdef DO_MAP_DEBUG
      printf("remap %s to %zu wants to be at %zu\n",tmp[i1].key,i2,tmp[i1].hash % map->size);
#endif /* DO_MAP_DEBUG */
      map->items[i2].key = tmp[i1].key;
      map->items[i2].value = tmp[i1].value;
      map->items[i2].hash = tmp[i1].hash;
    }
  }
#ifdef DO_MAP_DEBUG
  debug_print_map(map,"after rehash()");
#endif /* DO_MAP_DEBUG */
}

/* prototypes in "map.h" */
map_t* create_map(size_t size) {
  // should work
  map_t* map;
  size_t i;
  map = (map_t*)check_malloc(sizeof(map_t));
  map->size = size > MIN_MAP_SIZE ? size : MIN_MAP_SIZE;
  map->count = 0;
  map->index = 0;
  map->items = (key_value_t*)check_malloc(sizeof(key_value_t) * map->size);
  for(i = 0; i < map->size; i++) {
    map->items[i].key = NULL;
    map->items[i].value = NULL;
    map->items[i].hash = 0;
  }
#ifdef DO_MAP_DEBUG
  printf("*** MAP DEBUG CREATE ***\ncreated map at %p\nmap size: %zu\n",map,map->size);
#endif /* DO_MAP_DEBUG */
  return map;
}

void* add_value(map_t* map, const char* key, void* value) {
  // should work
  size_t idx;
  size_t hash;
  size_t start;
  hash = get_hash(key);
  start = hash % map->size;
  idx = start;
#ifdef DO_MAP_DEBUG
  printf("*** MAP DEBUG ADD ***\nwill add value: %p with key: %s\n",value,key);
#endif /* DO_MAP_DEBUG */
  while(1) {
    if(is_empty_item(map->items[idx])) {
      /* insert item */
      map->items[idx].key = strdup(key);
      map->items[idx].value = value;
      map->items[idx].hash = hash;
      map->count++;
      if((map->count + MAP_THRESHOLD) >= map->size) {
        rehash(map, map->size + 2 * MAP_THRESHOLD);
      }
      return NULL;
    } else {
      if(strcmp(map->items[idx].key, key) == 0) {
        /* update item */
        void* old_value;
        old_value = map->items[idx].value;
        map->items[idx].value = value;
        return old_value;
      }
    }
    idx++;
    if(idx >= map->size) {
      idx = 0;
    }
#ifdef MAP_PARANOID_INFINITY_LOOP_CHECK
    if(start == idx) {
      error_exit("add_or_set_value() failed! (infinity loop)\n");
    }
#endif /* MAP_PARANOID_INFINITY_LOOP_CHECK */
  }
#ifdef DO_MAP_DEBUG
  debug_print_map(map,"after add_value()");
#endif /* DO_MAP_DEBUG */
}

void* delete_value(map_t* map, const char* key) {
  // should work
  size_t idx;
  size_t start;
#ifdef DO_MAP_DEBUG
  printf("*** MAP DEBUG DELETE ***\nwill delete key %s\n",key);
#endif /* DO_MAP_DEBUG */
  start = get_hash(key) % map->size;
  idx = start;
  while(1) {
    if(is_empty_item(map->items[idx])) {
      return NULL; // nothing to delete
    } else {
      if(strcmp(map->items[idx].key, key) == 0) {
        void* tmp;
        tmp = map->items[idx].value;
        free(map->items[idx].key);
        map->items[idx].key = NULL;
        map->items[idx].value = NULL;
        map->items[idx].hash = 0;
        map->count--;
        // TODO: shrink map (or maybe not??)
        rehash(map, map->size);
        return tmp;
      }
    }
    idx++;
    if(idx >= map->size) {
      idx = 0;
    }
#ifdef MAP_PARANOID_INFINITY_LOOP_CHECK
    if(start == idx) {
      error_exit("delete_value() failed! (infinity loop)\n");
    }
#endif /* MAP_PARANOID_INFINITY_LOOP_CHECK */
  }
#ifdef DO_MAP_DEBUG
  debug_print_map(map,"after delete_value()");
#endif /* DO_MAP_DEBUG */
}

void* get_value(map_t* map, const char* key) {
  // should work
  size_t idx;
  size_t start;
  start = get_hash(key) % map->size;
  idx = start;
  while(1) {
    if(is_empty_item(map->items[idx])) {
      return NULL;
    } else {
      if(strcmp(map->items[idx].key, key) == 0) {
        return map->items[idx].value;
      }
    }
    idx++;
    if(idx >= map->size) {
      idx = 0;
    }
#ifdef MAP_PARANOID_INFINITY_LOOP_CHECK
    if(start == idx) {
      error_exit("get_value() failed! (infinity loop)\n");
    }
#endif /* MAP_PARANOID_INFINITY_LOOP_CHECK */
  }
}

void* iter_first(map_t* map) {
  size_t idx = 0;
  map->index = idx;
  while(idx < map->size) {
    if(!is_empty_item(map->items[idx])) {
      map->index = idx;
      return map->items[idx].value;
    }
    idx++;
  }
  return NULL;
}

void* iter_next(map_t* map) {
  size_t idx;
  idx = map->index + 1;
  while(idx < map->size) {
    if(!is_empty_item(map->items[idx])) {
      map->index = idx;
      return map->items[idx].value;
    }
    idx++;
  }
  return NULL;
}

void free_map(map_t* map, int free_values) {
  if(free_values != 0) {
    void* tmp;
    tmp = iter_first(map);
    if(tmp != NULL) {
      do {
        free(tmp);
        tmp = iter_next(map);
      } while(tmp != NULL);
    }
  }
  free(map->items);
  free(map);
}


#ifdef FNV_PRIME_32
#  undef FNV_PRIME_32
#endif /* FNV_PRIME_32 */

#ifdef FNV_OFFSET_BASIS_32
#  undef FNV_OFFSET_BASIS_32
#endif /* FNV_OFFSET_BASIS_32 */

#ifdef MAP_PARANOID_INFINITY_LOOP_CHECK
#  undef MAP_PARANOID_INFINITY_LOOP_CHECK
#endif /* MAP_PARANOID_INFINITY_LOOP_CHECK */

#ifdef DO_MAP_DEBUG
#  undef DO_MAP_DEBUG
#endif /* DO_MAP_DEBUG */

#ifdef MAP_THRESHOLD
#  undef MAP_THRESHOLD
#endif /* MAP_THRESHOLD */

#ifdef MIN_MAP_SIZE
#  undef MIN_MAP_SIZE
#endif /* MIN_MAP_SIZE */
