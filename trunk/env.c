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

/* lib */
#include <stdlib.h>

/* own */
#include "utils.h"
#include "map.h"
#include "ast.h"

/* self */
#include "env.h"

/* prototypes in "env.h" */
env_t* create_env(void) {
  env_t* env;
  env = (env_t*)check_malloc(sizeof(env_t));
  env->map = create_map(0);
  env->parent = NULL;
  return env;
}

void free_env(env_t* env) {
  ast_t* tmp;
  if((tmp = iter_first(env->map)) != NULL) {
    do {
      //      dec_ref(tmp);
    } while((tmp = iter_next(env->map)) != NULL);
  }
  free_map(env->map, 0);
  free(env);
}

ast_t* get_ast_by_id(env_t* env, const char* id) {
  ast_t* ast;
  ast = (ast_t*)get_value(env->map, id);
  if(ast == NULL && env->parent != NULL) {
    ast = get_ast_by_id(env->parent, id);
  }
  return ast;
}

void set_ast_to_id(env_t* env, const char* id, ast_t* ast) {
  ast_t* old;
  old = (ast_t*)add_value(env->map, id, ast);
  inc_ref(ast);
  if(old != NULL) {
    dec_ref(old);
  }
}

