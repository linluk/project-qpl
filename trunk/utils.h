#ifndef __UTILS_H__
#define __UTILS_H__

#include <stdlib.h>

/* if allocation fails program terminates! */
void* check_malloc(size_t size);
void* check_realloc(void* ptr, size_t size);

/* sets the pointer to NULL and frees the allocated value */
void free_null(void** p);

#endif /* __UTILS_H__ */
