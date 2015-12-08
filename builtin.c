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
 * Source code can be found under: <https://github.com/linluk/project-qpl/>.
 *
 ****************************************************************************/

/* lib */
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <inttypes.h>
#include <limits.h>

/* own */
#include "ast.h"
#include "env.h"
#include "utils.h"
#include "error.h"

/* self */
#include "builtin.h"

void populate_env(env_t* env) {
  /* stdin stdout */
  set_ast_to_id(env, "print", create_builtin_1(&builtin_print));
  set_ast_to_id(env, "println", create_builtin_1(&builtin_println));
  set_ast_to_id(env, "read", create_builtin_0(&builtin_read));
  set_ast_to_id(env, "readln", create_builtin_0(&builtin_readln));

  /* conversion */
  set_ast_to_id(env, "str", create_builtin_1(&builtin_to_string));
  set_ast_to_id(env, "int", create_builtin_1(&builtin_to_integer));
  set_ast_to_id(env, "dbl", create_builtin_1(&builtin_to_double));

  set_ast_to_id(env, "replace", create_builtin_3(&builtin_replace));

  set_ast_to_id(env, "typeof", create_builtin_1(&builtin_type_of));
  set_ast_to_id(env, "run", create_builtin_1(&builtin_run));

  set_ast_to_id(env, "fread", create_builtin_1(&builtin_fread));
  set_ast_to_id(env, "fwrite", create_builtin_2(&builtin_fwrite));
}

ast_t* builtin_print(ast_t* ast) {
  ast_t* str;
  str = builtin_to_string(ast);
  printf("%s",str->data.s);
  free_ast(str);
  return NULL;
}

ast_t* builtin_println(ast_t* ast) {
  ast_t* result;
  result = builtin_print(ast);
  printf("\n");
  return result;
}

ast_t* builtin_read(void) {
  ast_t* result;
  int c;
  char* s;
  c = fgetc(stdin);
  if(c >= 0) {
    s = (char*)check_malloc(2 * sizeof(char));
    s[1] = '\0';
    s[0] = (char)((c > CHAR_MAX) ? (c - (UCHAR_MAX + 1)) : c);
  } else {
    s = (char*)check_malloc(1 * sizeof(char));
    s[0] = '\0';
  };
  result = create_string(s);
  result->ref_count = 0;
  return result;
}

ast_t* builtin_readln(void) {
#define CHUNK_SIZE (80)
  ast_t* result = NULL;
  size_t size = CHUNK_SIZE;
  size_t idx = 0;
  char* s = (char*)check_malloc(size * sizeof(char));
  int c = fgetc(stdin);
  while(c >= 0 && c != '\n') {
    s[idx] = (char)((c > CHAR_MAX) ? (c - (UCHAR_MAX + 1)) : c);
    idx++;
    if(idx >= size) {
      size += CHUNK_SIZE;
      s = (char*)check_realloc(s,size * sizeof(char));
    }
    c = fgetc(stdin);
  }
  s = (char*)check_realloc(s,(idx + 1) * sizeof(char));
  s[idx] = '\0';
  result = create_string(s);
  result->ref_count = 0;
  return result;
#undef CHUNK_SIZE
}

ast_t* builtin_to_string(ast_t* ast) {
  ast_t* str = NULL;
  char buf[50];
  switch(ast->type) {
    case at_bool: sprintf(buf,"%s",ast->data.b == 0 ? "false" : "true"); break;
    case at_double: sprintf(buf,"%Lf",ast->data.d); break;
    case at_integer: sprintf(buf,"%jd",ast->data.i); break;
    case at_string: str = create_string(strdup(ast->data.s)); break;
    case at_function: sprintf(buf,"function <%p>",ast); break;
    default: error_convert(NULL,get_ast_type_name(ast->type),get_ast_type_name(at_string));
  }
  if(str == NULL) {
    str = create_string(strdup(buf));
  }
  str->ref_count = 0;
  return str;
}

ast_t* builtin_to_integer(ast_t* ast) {
  ast_t* num = NULL;
  switch(ast->type) {
    case at_bool: num = create_integer(ast->data.b == 0 ? 0 : 1); break;
    case at_double: num = create_integer((intmax_t)ast->data.d); break;
    case at_integer: num = create_integer(ast->data.i); break;
    case at_string:
      if(is_str_int(ast->data.s)) {
        num = create_integer(strtoimax(ast->data.s,NULL,10));
      } else {
        error_convert(NULL,get_ast_type_name(ast->type),get_ast_type_name(at_integer));
      }
      break;
    default: error_convert(NULL,get_ast_type_name(ast->type),get_ast_type_name(at_integer));
  }
  num->ref_count = 0;
  return num;
}

ast_t* builtin_to_double(ast_t* ast) {
  ast_t* dbl = NULL;
  switch(ast->type) {
    case at_bool: dbl = create_double(ast->data.b == 0 ? 0.0 : 1.0); break;
    case at_double: dbl = create_double(ast->data.d); break;
    case at_integer: dbl = create_double((long double)ast->data.i); break;
    case at_string:
      if(is_str_dbl(ast->data.s)) {
        dbl = create_double(atof(ast->data.s));
      } else {
        error_convert(NULL,get_ast_type_name(ast->type),get_ast_type_name(at_integer));
      }
      break;
    default: error_convert(NULL,get_ast_type_name(ast->type),get_ast_type_name(at_integer));
  }
  dbl->ref_count = 0;
  return dbl;
}

ast_t* builtin_to_bool(ast_t* ast) {
  ast_t* bol = NULL;
  switch(ast->type) {
    case at_bool: bol = create_bool(ast->data.b); break;
    case at_double: bol = create_bool(ast->data.d != 0.0); break;
    case at_integer: bol = create_bool(ast->data.i != 0); break;
    case at_string: bol = create_bool(strlen(ast->data.s) != 0); break;
    default: error_convert(NULL, get_ast_type_name(ast->type),get_ast_type_name(at_bool));
  }
  bol->ref_count = 0;
  return bol;
}

ast_t* builtin_replace(ast_t* str, ast_t* old, ast_t* new) {
  /* TODO : implement for lists too */
  ast_t* s = builtin_to_string(str);
  ast_t* o = builtin_to_string(old);
  ast_t* n = builtin_to_string(new);
  ast_t* result = create_string(replace_str(s->data.s, o->data.s, n->data.s));
  result->ref_count = 0;
  dec_ref(s);
  dec_ref(o);
  dec_ref(n);
  return result;
}

ast_t* builtin_type_of(ast_t* var) {
  char* typ = strdup(get_ast_type_name(var->type));
  ast_t* result = create_string(typ);
  result->ref_count = 0;
  return result;
}

ast_t* builtin_run(ast_t* command) {
#define CHUNK_SIZE (80)
  if(command->type != at_string) {
    error_expected(NULL, get_ast_type_name(at_string), get_ast_type_name(command->type));
  }
  /* whats with stderr? do i need stderr too?? this only gives me stdout */
  FILE* pipe = popen(command->data.s, "r");
  if(pipe == NULL) {
    error_failed(NULL,"popen()");
  }
  size_t size = CHUNK_SIZE;
  size_t idx = 0;
  char* s = (char*)check_malloc(size * sizeof(char));
  int c = fgetc(pipe);
  while(c != EOF) {
    s[idx] = (char)((c > CHAR_MAX) ? (c - (UCHAR_MAX + 1)) : c);
    idx++;
    if(idx >= size) {
      size += CHUNK_SIZE;
      s = (char*)check_realloc(s,size * sizeof(char));
    }
    c = fgetc(pipe);
  }
  pclose(pipe);
  s = (char*)check_realloc(s,(idx + 1) * sizeof(char));
  s[idx] = '\0';
  ast_t* result = create_string(s);
  result->ref_count = 0;
  return result;
#undef CHUNK_SIZE
}

ast_t* builtin_fread(ast_t* filename) {
#define CHUNK_SIZE (256)
  if(filename->type != at_string) {
    error_expected(NULL, get_ast_type_name(at_string), get_ast_type_name(filename->type));
  }
  FILE* file = fopen(filename->data.s, "r");
  if(file == NULL) {
    error_failed(NULL, "fopen()");
  }
  size_t size = CHUNK_SIZE;
  size_t idx = 0;
  char* s = (char*)check_malloc(size * sizeof(char));
  int c;
  while( (c = fgetc(file)) != EOF ) {
    s[idx] = (char)((c > CHAR_MAX) ? (c - (UCHAR_MAX + 1)) : c);
    idx++;
    if(idx >= size) {
      size += CHUNK_SIZE;
      s = (char*)check_realloc(s,size * sizeof(char));
    }
  }
  fclose(file);
  s = (char*)check_realloc(s, (idx + 1) * sizeof(char));
  s[idx] = '\0';
  ast_t* result = create_string(s);
  result->ref_count = 0;
  return result;
#undef CHUNK_SIZE
}

ast_t* builtin_fwrite(ast_t* filename, ast_t* content) {
  if(filename->type != at_string) {
    error_expected(NULL, get_ast_type_name(at_string), get_ast_type_name(filename->type));
  }
  if(content->type != at_string) {
    error_expected(NULL, get_ast_type_name(at_string), get_ast_type_name(content->type));
  }
  FILE* file = fopen(filename->data.s, "w");
  if(file == NULL) {
    error_failed(NULL, "fopen()");
  }
  fprintf(file, "%s", content->data.s);
  //fprintf(stdout, "%s", content->data.s);
//  char c;
//  size_t idx = 0;
//  while( (c = content->data.s[idx]) != '\0') {
//    fputc(c, file);
//    idx++;
//  }
  fclose(file);
  return NULL;
}

