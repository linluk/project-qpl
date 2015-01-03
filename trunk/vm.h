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

#ifndef __VM_H__
#define __VM_H__

#include "map.h"
#include "ast.h"
#include "env.h"

void vm_exec(env_t* env, ast_t* ast);

void exec_statements(env_t* env, ast_t* ast);
void exec_assignment(env_t* env, ast_t* ast);
void exec_conditional(env_t* env, ast_t* ast);
void exec_while(env_t* env, ast_t* ast);
void exec_dowhile(env_t* env, ast_t* ast);

int exec_if(env_t* env, ast_t* ast);

ast_t* eval_call(env_t* env, ast_t* ast);
ast_t* eval_expression(env_t* env, ast_t* ast);

#endif /* __VM_H__ */
