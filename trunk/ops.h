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

#ifndef __OPS_H__
#define __OPS_H__

#include "env.h"
#include "ast.h"

/* math ops */
ast_t* eval_add(env_t* env, ast_t* ast1, ast_t* ast2);
ast_t* eval_sub(env_t* env, ast_t* ast1, ast_t* ast2);
ast_t* eval_mul(env_t* env, ast_t* ast1, ast_t* ast2);
ast_t* eval_div(env_t* env, ast_t* ast1, ast_t* ast2);

/* logical ops */
ast_t* eval_and(env_t* env, ast_t* ast1, ast_t* ast2);

/* string ops */
ast_t* eval_cat(env_t* env, ast_t* ast1, ast_t* ast2);


#endif /* __OPS_H__ */
