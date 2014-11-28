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
void* add_value(map_t* map, char* key, void* value);

void* delete_value(map_t* map, char* key);

void* get_value(map_t* map, char* key);

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
