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
#include <stdlib.h>
#include <stdio.h>

/* own */
#include "ast.h"
#include "ops.h"
#include "env.h"
#include "utils.h"
#include "error.h"

/* self */
#include "vm.h"

ast_t* eval_call(env_t* env, ast_t* ast) {
  ast_t* func;
  ast_t* result;
  char* fn;  /* function name */
  func = NULL;
  result = NULL;
  fn = NULL;
  switch(ast->data.call.call_type) {
    case ct_anonymous:
      fn = "<anonymous>";
      func = ast->data.call.function.function;
      break;
    case ct_named:
      fn = ast->data.call.function.id;
      func = get_ast_by_id(env, fn);
      if(func == NULL) {
        error_id(NULL, fn);
      };
      break;
  }
  switch(func->type) {
    case at_function:{
      env_t* inner;
      size_t i;
      if(ast->data.call.callargs->data.callargs.count != func->data.function.params->data.params.count) {
        error_paramcount(NULL, fn, func->data.function.params->data.params.count, ast->data.call.callargs->data.callargs.count);
      }
      inner = create_env();
      inner->parent =  env;
      for(i = 0; i < func->data.function.params->data.params.count; i++) {
        set_ast_to_id(inner, func->data.function.params->data.params.params[i], eval_expression(env,ast->data.call.callargs->data.callargs.callargs[i]));
      }
      /* execute the function */
      exec_statements(inner, func->data.function.statements);
      /* get the result */
      inner->parent = NULL; /* must be NULL, get_ast_by_id() also searches the parent environment */
      result = get_ast_by_id(inner, "@");
      free_env(inner);
      break;
    }
    case at_builtin:
      if(ast->data.call.callargs->data.callargs.count != func->data.builtin.paramcount) {
        error_paramcount(NULL, fn, func->data.function.params->data.params.count, ast->data.call.callargs->data.callargs.count);
      }
      switch(func->data.builtin.paramcount) {
        case 0:
          result = func->data.builtin.function.builtin_0();
          break;
        case 1: {
          ast_t* p;
          p = eval_expression(env,ast->data.call.callargs->data.callargs.callargs[0]);
          result = func->data.builtin.function.builtin_1(p);
          dec_ref(p);
          break;
        }
        case 2: {
          ast_t* p1;
          ast_t* p2;
          p1 = eval_expression(env,ast->data.call.callargs->data.callargs.callargs[0]);
          p2 = eval_expression(env,ast->data.call.callargs->data.callargs.callargs[1]);
          result = func->data.builtin.function.builtin_2(p1,p2);
          dec_ref(p1);
          dec_ref(p2);
          break;
        }
        case 3: {
          ast_t* p1;
          ast_t* p2;
          ast_t* p3;
          p1 = eval_expression(env, ast->data.call.callargs->data.callargs.callargs[0]);
          p2 = eval_expression(env, ast->data.call.callargs->data.callargs.callargs[1]);
          p3 = eval_expression(env, ast->data.call.callargs->data.callargs.callargs[2]);
          result = func->data.builtin.function.builtin_3(p1,p2,p3);
          dec_ref(p1);
          dec_ref(p2);
          dec_ref(p3);
          break;
        }
        default:
          printf("\n\n*** HINT TO DEVELOPER ***\nimplement builtincall in vm.c\n\n");
          exit(1);
          /* if you create a builtin function with more parameters then you have to add a case here */
          break;
      }
      break;
    default:
      error_expected(NULL, get_ast_type_name(at_function), get_ast_type_name(func->type));
      break;
  }

  return result;
}

ast_t* eval_expression(env_t* env, ast_t* ast) {
  switch(ast->type) {
    /* valid */
    case at_call: return eval_call(env,ast);
    case at_identifier: return get_ast_by_id(env, ast->data.id);
    case at_expression: {
      ast_t* result;
      ast_t* left;
      ast_t* right;
      result = NULL;
      left = eval_expression(env, ast->data.expression.left);
      right = eval_expression(env, ast->data.expression.right);
      inc_ref(left);
      inc_ref(right);
      switch(ast->data.expression.op) {
        case op_add: result = eval_add(env, left, right); break;
        case op_mul: result = eval_mul(env, left, right); break;
        case op_div: result = eval_div(env, left, right); break;
        case op_sub: result = eval_sub(env, left, right); break;
        case op_mod: result = eval_mod(env, left, right); break;
        case op_and: result = eval_and(env, left, right); break;
        case op_or: result = eval_or(env, left, right); break;
        case op_gt: result = eval_gt(env, left, right); break;
        case op_ge: result = eval_ge(env, left, right); break;
        case op_lt: result = eval_lt(env, left, right); break;
        case op_le: result = eval_le(env, left, right); break;
        case op_eq: result = eval_eq(env, left, right); break;
        case op_neq: result = eval_neq(env, left, right); break;
        case op_cat: result = eval_cat(env, left, right); break;
      }
      result->ref_count = 0;
      dec_ref(left);
      dec_ref(right);
      return result;
    }
    /* this doesn't exist anymore!
    case at_new:{
      // TODO : implementieren.
      ast_t* result;
      ast_t* datadef;
      env_t* dataenv;
      datadef = eval_expression(env,ast->data.newop.datadef);
      inc_ref(datadef);
      dataenv = create_env();
      dataenv->parent = env;
      exec_statements(dataenv, datadef);
      result = create_instance(dataenv);
      result->ref_count = 0;
      dec_ref(datadef);
      return result;
    }*/
    /* no need to evaluate */
    case at_integer:
    case at_bool:
    case at_double:
    case at_string:
    case at_function:
    case at_statements:
    case at_list:
      return ast;

    /* invalid */
    case at_assignment:
    case at_callargs:
    case at_conditional:
    case at_dowhile:
    case at_elif:
    case at_if:
    case at_params:
    case at_while:
    case at_builtin:
      error_expected(NULL,"expression",get_ast_type_name(ast->type));
  }
  return NULL; /* this should never happen */
}

int exec_if(env_t* env, ast_t* ast) {
  ast_t* cond;
  int result; /* not null when condition was true,
                 null when condition was false */
  result = 0;
  cond = eval_expression(env, ast->data.if_statement.condition);
  if(cond->type != at_bool) {
    error_expected(NULL,get_ast_type_name(at_bool),get_ast_type_name(cond->type));
  } else {
    if(cond->data.b) {
      exec_statements(env, ast->data.if_statement.statements);
      result = 1;
    }
    dec_ref(cond);
  }
  return result;
}

void exec_conditional(env_t* env, ast_t* ast) {
  /* if */
  if(exec_if(env, ast->data.conditional.if_statement)) {
    return;
  } else {
    if(ast->data.conditional.elif_statements != NULL) {
      size_t i;
      /* elif */
      for(i = 0; i < ast->data.conditional.elif_statements->data.elif_statements.count; i++) {
        if(exec_if(env, ast->data.conditional.elif_statements->data.elif_statements.elif_statements[i])) {
          return;
        }
      }
    }
    if(ast->data.conditional.else_statement != NULL) {
      /* else */
      exec_statements(env, ast->data.conditional.else_statement);
    }
  }
}

void exec_while(env_t* env, ast_t* ast) {
  ast_t* cond;
  cond = eval_expression(env, ast->data.while_statement.condition);
  if(cond->type != at_bool) {
    error_expected(NULL,get_ast_type_name(at_bool),get_ast_type_name(cond->type));
  } else {
    while(cond->data.b) {
      exec_statements(env, ast->data.while_statement.statements);
      dec_ref(cond);
      cond = eval_expression(env, ast->data.while_statement.condition);
    }
    dec_ref(cond);
  }
}

void exec_dowhile(env_t* env, ast_t* ast) {
  /* same as exec_while() but with a call of exec_statements() before. */
  ast_t* cond;
  exec_statements(env, ast->data.while_statement.statements);
  cond = eval_expression(env, ast->data.while_statement.condition);
  if(cond->type != at_bool) {
    error_expected(NULL,get_ast_type_name(at_bool),get_ast_type_name(cond->type));
  } else {
    while(cond->data.b) {
      exec_statements(env, ast->data.while_statement.statements);
      dec_ref(cond);
      cond = eval_expression(env, ast->data.while_statement.condition);
    }
    dec_ref(cond);
  }
}

void exec_statements(env_t* env, ast_t* ast) {
  size_t i;
  for(i = 0; i < ast->data.statements.count; i++) {
    switch(ast->data.statements.statements[i]->type) {
      case at_assignment:
        exec_assignment(env, ast->data.statements.statements[i]);
        break;
      case at_call: {
        ast_t* tmp;
        tmp = eval_call(env, ast->data.statements.statements[i]);
        if(tmp != NULL) {
          if(tmp->ref_count == 0) {
            free_ast(tmp);
          }
        }
        break;
      }
      case at_conditional:
        exec_conditional(env, ast->data.statements.statements[i]);
        break;
      case at_while:
        exec_while(env, ast->data.statements.statements[i]);
        break;
      case at_dowhile:
        exec_dowhile(env, ast->data.statements.statements[i]);
        break;
      default:
        error_unexpected(NULL,get_ast_type_name(ast->data.statements.statements[i]->type));
        break;
    }
  }
}

void exec_assignment(env_t* env, ast_t* ast) {
  ast_t* right;
  right = NULL;
  switch(ast->data.assignment.right->type) {
    case at_function:
      right = ast->data.assignment.right;
      break;
    case at_bool:
    case at_double:
    case at_integer:
    case at_string:
    case at_statements:
      right = ast->data.assignment.right;
      break;
    case at_identifier:
      right = get_ast_by_id(env, ast->data.assignment.right->data.id);
      break;
    case at_expression:
      right = eval_expression(env, ast->data.assignment.right);
      break;
    case at_call:
      right = eval_call(env, ast->data.assignment.right);
      break;
    default:
      break;
  }
  if(right == NULL) {
    error_assign(NULL,"NULL",ast->data.assignment.id);
  }
  set_ast_to_id(env, ast->data.assignment.id, right);
}

void vm_exec(env_t* env, ast_t* ast) {
  if(ast->type == at_statements) {
    exec_statements(env,ast);
  } else {
    // TODO : -----
  }
}


