/*****************************************************************************
 *
 * Copyright (C) 2014 - 2016 Lukas Singer
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

#ifndef __AST_H__
#define __AST_H__

/* lib */
#include <stdint.h>

/* own */
#include "map.h"

typedef enum ast_type_e {
  at_identifier,
  at_integer, at_double, at_string, at_bool,
  at_list,
  at_expression,
  at_assignment,
  at_call, at_callargs,
  at_statements,
  at_conditional,
  at_if, at_elif,
  at_while, at_dowhile,
  at_function, at_params,
  at_builtin,
    /* add values before this */
} ast_type_t;

typedef enum call_type_e {
  ct_named,    /* "normal" function call (f.e: str(), doSomething(), ...) */
  ct_anonymous /* anonymous call (f.e:  fun(x){@=x*x;}(3), ...) */
} call_type_t;

typedef enum operator_e {
  op_add, op_sub,
  op_mul, op_div, op_mod,
  op_lt, op_gt, op_le, op_ge,
  op_eq, op_neq,
  op_and, op_or,
  op_cat,
  op_deref
} operator_t;

typedef struct position_s {
  char* file;
  int line;
} position_t;

/* THE abstract syntax tree structure */
typedef struct ast_s {
  ast_type_t type;
  int ref_count; /* positive value: ref_count -> call free when it becomes zero
                    negative value: created by parser -> never call free */
  union { /* data */
    intmax_t i;
    long double d;
    char b;
    char* s;
    char* id;
    struct { /* list */
      size_t count;
      struct ast_s** elements;
    } list;
    struct { /* expression */
      operator_t op;
      struct ast_s* left;
      struct ast_s* right;
    } expression;
    struct { /* assignment */
      char* id;
      struct ast_s* right;
    } assignment;
    struct { /* statements */
      size_t count;
      struct ast_s** statements;
    } statements;
    struct { /* call */
      call_type_t call_type;
      union { /* call.function */
        char* id;                /* use this when call_type is ct_named */
        struct ast_s* function;  /* use this when call_type is ct_anonymous */
      } function;
      struct ast_s* callargs;
    } call;
    struct { /* callargs */
      size_t count;
      struct ast_s** callargs;
    } callargs;
    struct { /* conditional */
      struct ast_s* if_statement;
      struct ast_s* elif_statements;
      struct ast_s* else_statement;
    } conditional;
    struct { /* if_statement */
      struct ast_s* condition;
      struct ast_s* statements;
    } if_statement;
    struct { /* elif_statements */
      size_t count;
      struct ast_s** elif_statements;
    } elif_statements;
    struct { /* while_statement */
      struct ast_s* condition;
      struct ast_s* statements;
    } while_statement;
    struct { /* dowhile_statement */
      struct ast_s* condition;
      struct ast_s* statements;
    } dowhile_statement;
    struct { /* function */
      struct ast_s* params;
      struct ast_s* statements;
    } function;
    struct { /* params */
      size_t count;
      char** params;
    } params;
    struct { /* builtin */
      size_t paramcount;
      union { /* builtin.function */
        struct ast_s* (*builtin_0)();
        struct ast_s* (*builtin_1)(struct ast_s*);
        struct ast_s* (*builtin_2)(struct ast_s*, struct ast_s*);
        struct ast_s* (*builtin_3)(struct ast_s*, struct ast_s*, struct ast_s*);
      } function;
    } builtin;
  } data;
} ast_t;

ast_t* create_integer(intmax_t value);
ast_t* create_double(long double value);
ast_t* create_bool(char value);
ast_t* create_string(char* value);
ast_t* create_list(ast_t* list, ast_t* element);
ast_t* create_identifier(char* id);
ast_t* create_expression(operator_t op, ast_t* left, ast_t* right);
ast_t* create_assignment(char* id, ast_t* right);
ast_t* create_statement(ast_t* statements, ast_t* statement);
ast_t* create_call(char* id, ast_t* function, ast_t* callargs);
ast_t* create_callarg(ast_t* callargs, ast_t* callarg);
ast_t* create_conditional(ast_t* if_statement, ast_t* elif_statements, ast_t* else_statement);
ast_t* create_if(ast_t* condition, ast_t* statements);
ast_t* create_elif(ast_t* elif_statements, ast_t* elif_statement);
ast_t* create_while(ast_t* condition, ast_t* statements);
ast_t* create_dowhile(ast_t* condition, ast_t* statements);
ast_t* create_function(ast_t* params, ast_t* statements);
ast_t* create_param(ast_t* params, char* id);
ast_t* create_builtin_0(ast_t*(*builtin_0)());
ast_t* create_builtin_1(ast_t*(*builtin_1)(ast_t*));
ast_t* create_builtin_2(ast_t*(*builtin_2)(ast_t*,ast_t*));
ast_t* create_builtin_3(ast_t*(*builtin_3)(ast_t*,ast_t*,ast_t*));
const char* get_ast_type_name(ast_type_t ast);
const char* get_op_str(operator_t op);

void inc_ref(ast_t* ast);
void dec_ref(ast_t* ast);

char is_numeric_ast_type(ast_type_t ast);

void print_ast(ast_t* ast, int indent);

void free_ast(ast_t* ast);

#endif /* __AST_H__ */

