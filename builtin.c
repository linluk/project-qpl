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

/* lib */
#include <stdio.h>
#include <string.h>

/* own */
#include "ast.h"
#include "env.h"

/* self */
#include "builtin.h"

void populate_env(env_t* env) {
  set_ast_to_id(env,"print",create_builtin_1(&builtin_print));
  set_ast_to_id(env,"println",create_builtin_1(&builtin_println));
  set_ast_to_id(env,"to_string",create_builtin_1(&builtin_to_string));
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

ast_t* builtin_to_string(ast_t* ast) {
  ast_t* str;
  char buf[50];
  str = NULL;
  switch(ast->type) {
    case at_bool: sprintf(buf,"%s",ast->data.b == 0 ? "false\0" : "true\0"); break;
    case at_double: sprintf(buf,"%f",ast->data.d); break;
    case at_integer: sprintf(buf,"%d",ast->data.i); break;
    case at_string: str = create_string(strdup(ast->data.s)); break;
  }
  if(str == NULL) {
    str = create_string(strdup(buf));
  }
  str->ref_count = 0;
  return str;
}




