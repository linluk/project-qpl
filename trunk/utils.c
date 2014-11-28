
#include <stdlib.h>
#include <stdio.h>

#include "utils.h"

void __error_exit(char* emsg) {
  fprintf(stderr, "%s", emsg);
  exit(1);
}

void* check_malloc(size_t size) {
  void* p;
  p = malloc(size);
  if(p == NULL) {
    __error_exit("memory allocation failed!\n");
  }
  return p;
}

void* check_realloc(void* ptr, size_t size) {
  void* p;
  p = realloc(ptr,size);
  if(p == NULL) {
    __error_exit("memory allocation failed!\n");
  }
  return p;
}

void free_null(void** p) {
  if((*p) != NULL) {
    free(*p);
    *p = NULL;
  }
}
