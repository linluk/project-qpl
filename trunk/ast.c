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
#include <stdlib.h>
#include <string.h>

/* own */
#include "utils.h"
#include "error.h"

/* self */
#include "ast.h"

/* ast type names, these are returned by get_ast_type_name() */
static const char* at_identifier_str = "identifier";
static const char* at_integer_str = "integer";
static const char* at_double_str = "double";
static const char* at_string_str = "string";
static const char* at_bool_str = "bool";
static const char* at_expression_str = "expression";
static const char* at_assignment_str = "assignment";
static const char* at_call_str = "call";
static const char* at_callargs_str = "callargs";
static const char* at_statements_str = "statements";
static const char* at_conditional_str = "conditional";
static const char* at_if_str = "if";
static const char* at_elif_str = "elif";
static const char* at_while_str = "while";
static const char* at_dowhile_str = "do-while";
static const char* at_function_str = "function";
static const char* at_params_str = "params";
static const char* at_builtin_str = "builtin";
static const char* at_unknown_str = "unknown";

/* operator strings, these are returned by get_op_str() */
static const char* op_add_str = "+";
static const char* op_sub_str = "-";
static const char* op_mul_str = "*";
static const char* op_div_str = "/";
static const char* op_mod_str = "%";
static const char* op_lt_str = "<";
static const char* op_gt_str = ">";
static const char* op_le_str = "<=";
static const char* op_ge_str = ">=";
static const char* op_eq_str = "==";
static const char* op_neq_str = "!=";
static const char* op_and_str = "&";
static const char* op_or_str = "|";
static const char* op_cat_str = "$";
static const char* op_unknown_str = "unknown";

/* prototypes */
ast_t* create_ast(ast_type_t type);

/* prototypes in "astgen.c" */
ast_t* create_ast(ast_type_t type) {
  ast_t* ast;
  ast = check_malloc(sizeof(ast_t));
  ast->type = type;
  ast->ref_count = -1; /* created by parser --> never call free! */
  return ast;
}

/* prototypes in "astgen.h" */
ast_t* create_integer(int value) {
  ast_t* ast;
  ast = create_ast(at_integer);
  ast->data.i = value;
  return ast;  
}

ast_t* create_double(double value) {
  ast_t* ast;
  ast = create_ast(at_double);
  ast->data.d = value;
  return ast;
}

ast_t* create_bool(char value) {
  ast_t* ast;
  ast = create_ast(at_bool);
  ast->data.b = value;
  return ast;
}

ast_t* create_string(char* value) {
  ast_t* ast;
  ast = create_ast(at_string);
  ast->data.s = value;
  return ast;
}

ast_t* create_identifier(char* id) {
  ast_t* ast;
  ast = create_ast(at_identifier);
  ast->data.id = id;
  return ast;
}

ast_t* create_expression(operator_t op, ast_t* left, ast_t* right) {
  ast_t* ast;
  ast = create_ast(at_expression);
  ast->data.expression.op = op;
  ast->data.expression.left = left;
  ast->data.expression.right = right;
  return ast;
}

ast_t* create_assignment(char* id, ast_t* right) {
  ast_t* ast;
  ast = create_ast(at_assignment);
  ast->data.assignment.id = id;
  ast->data.assignment.right = right;
  return ast;
}
ast_t* create_statement(ast_t* statements, ast_t* statement) {
  if(statements == NULL) {
    statements = create_ast(at_statements);
    statements->data.statements.count = 0;
    statements->data.statements.statements = NULL;
  }
  if(statements->type != at_statements) {
    fprintf(stderr, "ast generation error\n");
    exit(1);
  }
  statements->data.statements.count++;
  statements->data.statements.statements = (ast_t**)check_realloc(statements->data.statements.statements, statements->data.statements.count * sizeof(ast_t*));
  statements->data.statements.statements[statements->data.statements.count - 1] = statement;
  return statements;
}

ast_t* create_call(char* id, ast_t* function, ast_t* callargs) {
  ast_t* ast;
  ast = create_ast(at_call);
  if(id != NULL) {
    ast->data.call.call_type = ct_named;
    ast->data.call.function.id = id;
  } else {
    if(function != NULL) {
      ast->data.call.call_type = ct_anonymous;
      ast->data.call.function.function = function;
    } else {
      fprintf(stderr, "ast generation error\n");
      exit(1);
    }
  }
  ast->data.call.callargs = callargs;
  return ast;
}

ast_t* create_callarg(ast_t* callargs, ast_t* callarg) {
  if(callargs == NULL) {
    callargs = create_ast(at_callargs);
    callargs->data.callargs.count = 0;
    callargs->data.callargs.callargs = NULL;
  }
  if(callargs->type != at_callargs) {
    fprintf(stderr, "ast generation error\n");
    exit(1);
  }
  callargs->data.callargs.count++;
  callargs->data.callargs.callargs = (ast_t**)check_realloc(callargs->data.callargs.callargs, callargs->data.callargs.count * sizeof(ast_t*));
  callargs->data.callargs.callargs[callargs->data.callargs.count - 1] = callarg;
  return callargs;
}

ast_t* create_conditional(ast_t* if_statement, ast_t* elif_statements, ast_t* else_statement) {
  ast_t* ast;
  ast = create_ast(at_conditional);
  ast->data.conditional.if_statement = if_statement;
  ast->data.conditional.elif_statements = elif_statements;
  ast->data.conditional.else_statement = else_statement;
  return ast;
}

ast_t* create_if(ast_t* condition, ast_t* statements) {
  ast_t* ast;
  ast = create_ast(at_if);
  ast->data.if_statement.condition = condition;
  ast->data.if_statement.statements = statements;
  return ast;
}

ast_t* create_elif(ast_t* elif_statements, ast_t* elif_statement) {
  if(elif_statements == NULL) {
    elif_statements = create_ast(at_elif);
    elif_statements->data.elif_statements.count = 0;
    elif_statements->data.elif_statements.elif_statements = NULL;
  }
  if(elif_statements->type != at_elif) {
    fprintf(stderr, "ast generation error\n");
    exit(1);
  }
  elif_statements->data.elif_statements.count++;
  elif_statements->data.elif_statements.elif_statements = (ast_t**)check_realloc(elif_statements->data.elif_statements.elif_statements, elif_statements->data.elif_statements.count * sizeof(ast_t*));
  elif_statements->data.elif_statements.elif_statements[elif_statements->data.elif_statements.count - 1] = elif_statement;
  return elif_statements;
}

ast_t* create_while(ast_t* condition, ast_t* statements) {
  ast_t* ast;
  ast = create_ast(at_while);
  ast->data.while_statement.condition = condition;
  ast->data.while_statement.statements = statements;
  return ast;
}

ast_t* create_dowhile(ast_t* condition, ast_t* statements) {
  ast_t* ast;
  ast = create_ast(at_dowhile);
  ast->data.dowhile_statement.condition = condition;
  ast->data.dowhile_statement.statements = statements;
  return ast;
}

ast_t* create_function(ast_t* params, ast_t* statements) {
  ast_t* ast;
  ast = create_ast(at_function);
  ast->data.function.params = params;
  ast->data.function.statements = statements;
  return ast;
}

ast_t* create_param(ast_t* params, char* param) {
  if(params == NULL) {
    params = create_ast(at_params);
    params->data.params.count = 0;
    params->data.params.params = NULL;
  }
  if(params->type != at_params) {
    fprintf(stderr, "ast generation error\n");
    exit(1);
  }
  params->data.params.count++;
  params->data.params.params = (char**)check_realloc(params->data.params.params, params->data.params.count * sizeof(char*));
  params->data.params.params[params->data.params.count - 1] = param;
  return params;
}

ast_t* create_builtin_0(ast_t*(*builtin_0)()) {
  ast_t* ast;
  ast = create_ast(at_builtin);
  ast->data.builtin.paramcount = 0;
  ast->data.builtin.function.builtin_0 = builtin_0;
  return ast;
}

ast_t* create_builtin_1(ast_t*(*builtin_1)(ast_t*)) {
  ast_t* ast;
  ast = create_ast(at_builtin);
  ast->data.builtin.paramcount = 1;
  ast->data.builtin.function.builtin_1 = builtin_1;
  return ast;
}

ast_t* create_builtin_2(ast_t*(*builtin_2)(ast_t*,ast_t*)) {
  ast_t* ast;
  ast = create_ast(at_builtin);
  ast->data.builtin.paramcount = 2;
  ast->data.builtin.function.builtin_2 = builtin_2;
  return ast;
}


int is_numeric_ast_type(ast_type_t ast) {
  return (ast == at_double) || (ast == at_integer);
}

const char* get_ast_type_name(ast_type_t ast) {
  switch(ast) {
    case at_assignment: return at_assignment_str;
    case at_bool: return at_bool_str;
    case at_call: return at_call_str;
    case at_identifier: return at_identifier_str;
    case at_callargs: return at_callargs_str;
    case at_conditional: return at_conditional_str;
    case at_double: return at_double_str;
    case at_dowhile: return at_dowhile_str;
    case at_elif: return at_elif_str;
    case at_expression: return at_expression_str;
    case at_function: return at_function_str;
    case at_if: return at_if_str;
    case at_integer: return at_integer_str;
    case at_params: return at_params_str;
    case at_statements: return at_statements_str;
    case at_string: return at_string_str;
    case at_while: return at_while_str;
    case at_builtin: return at_builtin_str;
    default: return at_unknown_str;
  }
}

const char* get_op_str(operator_t op) {
  switch(op) {
    case op_add: return op_add_str;
    case op_sub: return op_sub_str;
    case op_mul: return op_mul_str;
    case op_div: return op_div_str;
    case op_mod: return op_mod_str;
    case op_lt: return op_lt_str;
    case op_gt: return op_gt_str;
    case op_le: return op_le_str;
    case op_ge: return op_ge_str;
    case op_eq: return op_eq_str;
    case op_neq: return op_neq_str;
    case op_and: return op_and_str;
    case op_or: return op_or_str;
    case op_cat: return op_cat_str;
    default: return op_unknown_str;
  }
}

void print_ast(ast_t* ast, int indent){
  size_t i;
  for(i = 0; i < indent; i++) {
    printf(" ");
  }
  if(ast != NULL) {
    const char* tn = get_ast_type_name(ast->type);
    switch(ast->type) {
      case at_identifier:
        printf("%s: %s\n",tn,ast->data.id);
        break;
      case at_integer:
        printf("%s: %d\n",tn,ast->data.i);
        break;
      case at_double:
        printf("%s: %f\n",tn,ast->data.d);
        break;
      case at_string:
        printf("%s: \"%s\"\n",tn,ast->data.s);
        break;
      case at_bool:
        printf("%s: %s\n",tn,ast->data.b > 0 ? "true" : "false");
        break;
      case at_expression:
        printf("%s: %s\n",tn,get_op_str(ast->data.expression.op));
        print_ast(ast->data.expression.left,indent+2);
        print_ast(ast->data.expression.right,indent+2);
        break;
      case at_assignment:
        printf("%s: %s\n",tn,ast->data.assignment.id);
        print_ast(ast->data.assignment.right,indent+2);
        break;
      case at_call:
        switch(ast->data.call.call_type) {
          case ct_anonymous:
            printf("%s: <anonymous>\n", tn);
            print_ast(ast->data.call.function.function, indent+2);
            break;
          case ct_named:
            printf("%s: %s\n",tn, ast->data.call.function.id);
            break;
        }
        print_ast(ast->data.call.callargs,indent+2);
        break;
      case at_callargs:
        printf("%s: %d\n",tn, ast->data.callargs.count);
        for(i = 0; i < ast->data.callargs.count; i++) {
          print_ast(ast->data.callargs.callargs[i],indent+2);
        }
        break;
      case at_statements:
        printf("%s:\n",tn);
        for(i = 0; i < ast->data.statements.count; i++) {
          print_ast(ast->data.statements.statements[i],indent+2);
        }
        break;
      case at_conditional:
        printf("%s:\n",tn);
        print_ast(ast->data.conditional.if_statement,indent+2);
        print_ast(ast->data.conditional.elif_statements,indent+2);
        print_ast(ast->data.conditional.else_statement,indent+2);
        break;
      case at_if:
        printf("%s:\n",tn);
        print_ast(ast->data.if_statement.condition,indent+2);
        print_ast(ast->data.if_statement.statements,indent+2);
        break;
      case at_elif:
        printf("%s: %d\n",tn,ast->data.elif_statements.count);
        for(i = 0; i < ast->data.elif_statements.count; i++) {
          print_ast(ast->data.elif_statements.elif_statements[i],indent+2);
        }
        break;
      case at_while:
        printf("%s:\n",tn);
        print_ast(ast->data.while_statement.condition,indent+2);
        print_ast(ast->data.while_statement.statements,indent+2);
        break;
      case at_dowhile:
        printf("%s:\n",tn);
        print_ast(ast->data.dowhile_statement.condition,indent+2);
        print_ast(ast->data.dowhile_statement.statements,indent+2);
        break;
      case at_function:
        printf("%s:\n",tn);
        print_ast(ast->data.function.params,indent+2);
        print_ast(ast->data.function.statements,indent+2);
        break;
      case at_builtin:
        printf("%s:\n",tn);
        break;
      case at_params:
        printf("%s: %d ( ",tn,ast->data.params.count);
        for(i = 0; i < ast->data.params.count; i++) {
          if(i != 0) {
            printf(", ");
          }
          printf(ast->data.params.params[i]);
        }
        printf(")\n");
        break;
      default:
        fprintf(stderr,"warning: unknown ast type: %s:%d\n", tn, (int)ast->type);
        exit(1);
        break;
    }
  } else {
    printf("/* empty */\n");
  }
}

void free_ast(ast_t* ast) {
  size_t i;
  if(ast != NULL) {
    switch(ast->type) {
      case at_identifier:
        free(ast->data.id);
        break;
      case at_integer:
        break;
      case at_double:
        break;
      case at_string:
        free(ast->data.s);
        break;
      case at_bool:
        break;
      case at_expression:
        free_ast(ast->data.expression.left);
        free_ast(ast->data.expression.right);
        break;
      case at_assignment:
        free(ast->data.assignment.id);
        free_ast(ast->data.assignment.right);
        break;
        break;
      case at_call:
        switch(ast->data.call.call_type) {
          case ct_anonymous:
            free_ast(ast->data.call.function.function);
            break;
          case ct_named:
            free(ast->data.call.function.id);
            break;
        }
        free_ast(ast->data.call.callargs);
        break;
      case at_callargs:
        for(i = 0; i < ast->data.callargs.count; i++) {
          free_ast(ast->data.callargs.callargs[i]);
        }
        break;
      case at_statements:
        for(i = 0; i < ast->data.statements.count; i++) {
          free_ast(ast->data.statements.statements[i]);
        }
        break;
      case at_conditional:
        free_ast(ast->data.conditional.if_statement);
        free_ast(ast->data.conditional.elif_statements);
        free_ast(ast->data.conditional.else_statement);
        break;
      case at_if:
        free_ast(ast->data.if_statement.condition);
        free_ast(ast->data.if_statement.statements);
        break;
      case at_elif:
        for(i = 0; i < ast->data.elif_statements.count; i++) {
          free_ast(ast->data.elif_statements.elif_statements[i]);
        }
        break;
      case at_while:
        free_ast(ast->data.while_statement.condition);
        free_ast(ast->data.while_statement.statements);
        break;
      case at_dowhile:
        free_ast(ast->data.dowhile_statement.condition);
        free_ast(ast->data.dowhile_statement.statements);
        break;
      case at_function:
        free_ast(ast->data.function.params);
        free_ast(ast->data.function.statements);
        break;
      case at_builtin:
        break;
      case at_params:
        for(i = 0; i < ast->data.params.count; i++) {
          free(ast->data.params.params[i]);
        }
        break;
      default:
        break;
    }
    free(ast);
  }
}

