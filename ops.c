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
#include "vm.h"
#include "env.h"
#include "error.h"
#include "utils.h"

/* self */
#include "ops.h"

/* prototypes */
ast_t* eval_math(env_t* env, operator_t op, ast_t* ast1, ast_t* ast2,double (math_func)(double,double));
double __add_func(double d1, double d2);
double __sub_func(double d1, double d2);
double __mul_func(double d1, double d2);
double __div_func(double d1, double d2);

ast_t* eval_gtlt(env_t* env, operator_t op, ast_t* ast1, ast_t* ast2, char (comp_func)(double,double));
char __gt_func(double d1, double d2);
char __lt_func(double d1, double d2);
char __ge_func(double d1, double d2);
char __le_func(double d1, double d2);
ast_t* eval_eqneq(env_t* env, operator_t op, ast_t* ast1, ast_t* ast2);

/* prototypes in "ops.c" */
ast_t* eval_math(env_t* env, operator_t op, ast_t* ast1, ast_t* ast2,double (math_func)(double,double)) {
  ast_t* result;
  result = NULL;
  switch(ast1->type) {
    case at_integer:
      switch(ast2->type) {
        case at_integer:
          result = create_integer((int)math_func((double)ast1->data.i, (double)ast2->data.i));
          break;
        case at_double:
          result = create_double(math_func((double)ast1->data.i, ast2->data.d));
          break;
        default:
          error_apply(NULL,get_op_str(op), get_ast_type_name(ast1->type), get_ast_type_name(ast2->type));
          break;
      }
      break;
    case at_double:
      switch(ast2->type) {
        case at_integer:
          result = create_double(math_func(ast1->data.d, (double)ast2->data.i));
          break;
        case at_double:
          result = create_double(math_func(ast1->data.d, ast2->data.d));
          break;
        default:
          error_apply(NULL,get_op_str(op), get_ast_type_name(ast1->type), get_ast_type_name(ast2->type));
          break;
      }
      break;
    default:
      error_apply(NULL,get_op_str(op), get_ast_type_name(ast1->type), get_ast_type_name(ast2->type));
      break;
  }
  return result;
}

double __add_func(double d1, double d2) {
  return d1 + d2;
}

double __sub_func(double d1, double d2) {
  return d1 - d2;
}

double __mul_func(double d1, double d2) {
  return d1 * d2;
}

double __div_func(double d1, double d2) {
  // TODO : div by zero
  return d1 / d2;
}

ast_t* eval_gtlt(env_t* env, operator_t op, ast_t* ast1, ast_t* ast2, char (comp_func)(double,double)) {
  ast_t* result;
  result = NULL;
  switch(ast1->type) {
    case at_integer:
      switch(ast2->type) {
        case at_integer:
          result = create_bool((int)comp_func((double)ast1->data.i, (double)ast2->data.i));
          break;
        case at_double:
          result = create_bool(comp_func((double)ast1->data.i, ast2->data.d));
          break;
        default:
          error_apply(NULL,get_op_str(op), get_ast_type_name(ast1->type), get_ast_type_name(ast2->type));
          break;
      }
      break;
    case at_double:
      switch(ast2->type) {
        case at_integer:
          result = create_bool(comp_func(ast1->data.d, (double)ast2->data.i));
          break;
        case at_double:
          result = create_bool(comp_func(ast1->data.d, ast2->data.d));
          break;
        default:
          error_apply(NULL,get_op_str(op), get_ast_type_name(ast1->type), get_ast_type_name(ast2->type));
          break;
      }
      break;
    default:
      error_apply(NULL,get_op_str(op), get_ast_type_name(ast1->type), get_ast_type_name(ast2->type));
      break;
  }
  return result;
}

char __gt_func(double d1, double d2) {
  return d1 > d2 ? 1 : 0;
}

char __lt_func(double d1, double d2) {
  return d1 < d2 ? 1 : 0;
}

char __ge_func(double d1, double d2) {
  return d1 >= d2 ? 1 : 0;
}

char __le_func(double d1, double d2) {
  return d1 <= d2 ? 1 : 0;
}

ast_t* eval_eqneq(env_t* env, operator_t op, ast_t* ast1, ast_t* ast2) {
  ast_t* result;
  result = NULL;
  if(ast1->type == ast2->type) {
    switch(ast1->type) {
      case at_bool: result = create_bool(ast1->data.b == ast2->data.b ? 1 : 0);
      case at_double: result = create_bool( ast1->data.d == ast2->data.d ? 1 : 0);
      case at_integer: result = create_bool( ast1->data.i == ast2->data.i ? 1 : 0);
      case at_string: result = create_bool( strcmp(ast1->data.s, ast2->data.s) == 0 ? 1 : 0);
      default: error_apply(NULL,get_op_str(op_eq), get_ast_type_name(ast1->type), get_ast_type_name(ast2->type));
    }
    if(op == op_neq) {
      result->data.b = result->data.b == 0 ? 1 : 0;
    }
  } else {
    error_apply(NULL,get_op_str(op_eq), get_ast_type_name(ast1->type), get_ast_type_name(ast2->type));
  }
  return result;
}


/* prototypes in "ops.h" */
/* math ops */
ast_t* eval_add(env_t* env, ast_t* ast1, ast_t* ast2) {
  return eval_math(env,op_add,ast1,ast2,&__add_func);
}

ast_t* eval_sub(env_t* env, ast_t* ast1, ast_t* ast2) {
  return eval_math(env,op_sub,ast1,ast2,&__sub_func);
}

ast_t* eval_mul(env_t* env, ast_t* ast1, ast_t* ast2) {
  return eval_math(env,op_mul,ast1,ast2,&__mul_func);
}

ast_t* eval_div(env_t* env, ast_t* ast1, ast_t* ast2) {
  return eval_math(env,op_div,ast1,ast2,&__div_func);
}

ast_t* eval_mod(env_t* env, ast_t* ast1, ast_t* ast2) {
  ast_t* result;
  result = NULL;
  if(ast1->type == at_integer && ast2->type == at_integer) {
    result = create_integer(ast1->data.i % ast2->data.i);
  } else {
    error_apply(NULL,get_op_str(op_mod), get_ast_type_name(ast1->type), get_ast_type_name(ast2->type));
  }
  return result;
}

/* logical ops */
ast_t* eval_and(env_t* env, ast_t* ast1, ast_t* ast2) {
  ast_t* result;
  result = NULL;
  if(ast1->type == at_bool && ast2->type == at_bool) {
    result = create_bool(ast1->data.b && ast2->data.b);
  } else {
    error_apply(NULL,get_op_str(op_and), get_ast_type_name(ast1->type), get_ast_type_name(ast2->type));
  }
  return result;
}

ast_t* eval_or(env_t* env, ast_t* ast1, ast_t* ast2) {
  ast_t* result;
  result = NULL;
  if(ast1->type == at_bool && ast2->type == at_bool) {
    result = create_bool(ast1->data.b || ast2->data.b);
  } else {
    error_apply(NULL,get_op_str(op_and), get_ast_type_name(ast1->type), get_ast_type_name(ast2->type));
  }
  return result;
}

/* compare ops */
ast_t* eval_gt(env_t* env, ast_t* ast1, ast_t* ast2) {
  return eval_gtlt(env, op_gt, ast1, ast2, &__gt_func);
}

ast_t* eval_lt(env_t* env, ast_t* ast1, ast_t* ast2) {
  return eval_gtlt(env, op_lt, ast1, ast2, &__lt_func);
}

ast_t* eval_ge(env_t* env, ast_t* ast1, ast_t* ast2) {
  return eval_gtlt(env, op_ge, ast1, ast2, &__ge_func);
}

ast_t* eval_le(env_t* env, ast_t* ast1, ast_t* ast2) {
  return eval_gtlt(env, op_ge, ast1, ast2, &__le_func);
}

ast_t* eval_eq(env_t* env, ast_t* ast1, ast_t* ast2) {
  return eval_eqneq(env, op_eq, ast1, ast2);
}

ast_t* eval_neq(env_t* env, ast_t* ast1, ast_t* ast2) {
  return eval_eqneq(env, op_neq, ast1, ast2);
}

/* string ops */
ast_t* eval_cat(env_t* env, ast_t* ast1, ast_t* ast2) {
  ast_t* result;
  result = NULL;
  if(ast1->type == at_string && ast2->type == at_string) {
    result = create_string(NULL);
    result->data.s = (char*)check_malloc((strlen(ast1->data.s) + strlen(ast2->data.s) + 1) * sizeof(char));
    strcpy(result->data.s, ast1->data.s);
    strcat(result->data.s, ast2->data.s);
  } else {
    error_apply(NULL,get_op_str(op_cat), get_ast_type_name(ast1->type), get_ast_type_name(ast2->type));
  }
  return result;
}



