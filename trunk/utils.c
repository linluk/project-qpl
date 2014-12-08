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
#include <string.h>
#include <stddef.h>


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

char is_str_int(const char* str) {
  if((str[0] == '-' || str[0] == '+') || (str[0] >= '0' && str[0] <= '9')) {
    size_t i;
    i = 1;
    while(str[i]) {
      if(str[i] < '0' || str[i] > '9') {
        return 0;
      }
      i++;
    }
  } else {
    return 0;
  }
  return 1;
}

char is_str_dbl(const char* str) {
  if((str[0] == '-' || str[0] == '+') || (str[0] >= '0' && str[0] <= '9')) {
    size_t i;
    char p;
    i = 1;
    p = 0;
    while(str[i]) {
      if(str[i] < '0' || str[i] > '9') {
        if(p == 0 && str[i] == '.') {
          p = 1;
        } else {
          return 0;
        }
      }
      i++;
    }
  } else {
    return 0;
  }
  return 1;  
}

char* replace_str(const char* str, const char* old, const char* new) {
  size_t olen;
  size_t nlen;
  size_t slen;
  size_t rlen;
  char* tmp;
  char* result;
  ptrdiff_t doffset;
  ptrdiff_t soffset;
  ptrdiff_t clen;
  int cnt;
  olen = strlen(old);
  nlen = strlen(new);
  slen = strlen(str);
  /* get count of replace patterns */
  cnt = 0;
  tmp = strstr(str,old);
  while(tmp != NULL) {
    cnt++;
    tmp = strstr((tmp+olen),old);
  }
  /* check if there is nothing to replace */
  if(cnt == 0) {
    result = strdup(str); /* copy the original string to result */
  } else {
    /* create a buffer for the "new" string */
    rlen = slen + cnt * (nlen - olen);
    result = (char*)check_malloc((1 + rlen)  * sizeof(char));
    doffset = 0;
    soffset = 0;
    clen = 0;
    tmp = strstr(str, old);
    while(tmp != NULL) {
      clen = tmp - (str + soffset);
      strncpy(result + doffset, str + soffset, clen);
      doffset += clen;
      soffset += clen + olen;
      strncpy(result + doffset, new, nlen);
      doffset += nlen;
      tmp = strstr(str + soffset, old);
    }
    strncpy(result + doffset, str + soffset, rlen - doffset);
    result[rlen] = '\0';
  }
  return result;
}



