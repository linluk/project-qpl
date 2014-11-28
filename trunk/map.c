
#include <stdlib.h>
#include <stdio.h>
#include <string.h> /* TODO: refactor hash from string functions to pointer arithmetic */

#include "utils.h"

#include "map.h"

/* defines */
#define FNV_PRIME_32 (16777619)
#define FNV_OFFSET_BASIS_32 (2166136261)

#define MAP_PARANOID_INFINITY_LOOP_CHECK

#define MAP_THRESHOLD (5)
#define MIN_MAP_SIZE (15)

/* prototypes */
size_t get_hash(char* key);

void error_exit(char* emsg);

int is_empty_item(key_value_t item);

void rehash(map_t* map, size_t new_size);  /* TODO: rename to repopulate */

/* prototypes in "map.c" */
size_t get_hash(char* key) {
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

void error_exit(char* emsg) {
  fprintf(stderr, "error: \"%s\"", emsg);
  exit(1);
}

int is_empty_item(key_value_t item) {
  return (item.key == NULL && item.hash == 0 && item.value == NULL);
}

void rehash(map_t* map, size_t new_size) {
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
    for(i1 = 0; i1 < map->count; i1++) {
      i2 = tmp[i1].hash % map->size;
      while(!is_empty_item(map->items[i2])) {
        i2++;
      }
      map->items[i2].key = tmp[i1].key;
      map->items[i2].value = tmp[i1].value;
      map->items[i2].hash = tmp[i1].hash;
    }
  }
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
  return map;
}

void* add_value(map_t* map, char* key, void* value) {
  // should work
  size_t idx;
  size_t hash;
  size_t start;
  hash = get_hash(key);
  start = hash % map->size;
  idx = start;
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
}

void* delete_value(map_t* map, char* key) {
  // should work
  size_t idx;
  size_t start;
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
}

void* get_value(map_t* map, char* key) {
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


