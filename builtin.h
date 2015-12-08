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

#ifndef __BUILTIN_H__
#define __BUILTIN_H__

#include "ast.h"
#include "env.h"

void populate_env(env_t* env);

/* stdin stdout */
ast_t* builtin_print(ast_t* ast);
ast_t* builtin_println(ast_t* ast);
ast_t* builtin_read(void);
ast_t* builtin_readln(void);

ast_t* builtin_to_string(ast_t* ast);
ast_t* builtin_to_integer(ast_t* ast);
ast_t* builtin_to_double(ast_t* ast);
ast_t* builtin_to_bool(ast_t* ast);

ast_t* builtin_replace(ast_t* str, ast_t* old, ast_t* new);

ast_t* builtin_type_of(ast_t* var);

ast_t* builtin_run(ast_t* command);

/* file i/o */
ast_t* builtin_fread(ast_t* filename);
ast_t* builtin_fwrite(ast_t* filename, ast_t* content);



#endif /* __BUILTIN_H__ */
