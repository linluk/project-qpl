
/* lib */
#include <stdlib.h>
#include <stdio.h>

/* own */
#include "map.h"
#include "ast.h"
#include "utils.h"
#include "error.h"

/* self */
#include "vm.h"

/* prototypes */

ast_t* create_ast(ast_type_t type);


void exec_statements(env_t* env, ast_t* ast);
void exec_assignment(env_t* env, ast_t* ast);
ast_t* eval_expression(env_t* env, ast_t* ast);
ast_t* eval_add(env_t* env, ast_t* ast1, ast_t* ast2);
ast_t* eval_and(env_t* env, ast_t* ast1, ast_t* ast2);

/* prototypes in "vm.c" */

ast_t* create_ast(ast_type_t type) {
  ast_t* ast;
  ast = check_malloc(sizeof(ast_t));
  ast->type = type;
  ast->ref_count = 1; /* created by execution --> call free when zero! */
  return ast;
}

ast_t* eval_add(env_t* env, ast_t* ast1, ast_t* ast2) {

}

ast_t* eval_and(env_t* env, ast_t* ast1, ast_t* ast2) {

}

ast_t* eval_expression(env_t* env, ast_t* ast) {
  switch(ast->type) {
    /* valid */
    case at_bool: return ast;
    case at_call: return exec_call(env,ast);
    case at_identifier: return (ast_t*)get_value(env->map, ast->data.id);
    case at_double: return ast;
    case at_expression: 
      switch(ast->data.expression.op) {
        case op_add: return eval_add(env, ast->data.expression.left, ast->data.expression.right);
        case op_and: return eval_and(env, ast->data.expression.left, ast->data.expression.right);

      }
    case at_integer: return ast;
    case at_string: return ast;

    /* invalid */
    case at_assignment:
    case at_callargs:
    case at_conditional:
    case at_dowhile:
    case at_elif:
    case at_end:
    case at_function:
    case at_if:
    case at_params:
    case at_statements:
    case at_while:
      error_expected(NULL,"expression",get_ast_type_name(ast->type));
  }
}

void exec_statements(env_t* env, ast_t* ast) {
  size_t i;
  for(i = 0; i < ast->data.statements.count; i++) {
    switch(ast->data.statements.statements[i]->type) {
      case at_assignment:
        exec_assignment(env, ast->data.statements.statements[i]);
        break;
      case at_call:  // TODO: hier weitermachen !!
        break;
      case at_conditional:
        break;
      case at_while:
        break;
      case at_dowhile:
        break;
      default:
        fprintf(stderr,"error, unexpected: \"%s\"\n",get_ast_type_name(ast->data.statements.statements[i]->type));
        exit(1);
        break;
    }
  }
}

void exec_assignment(env_t* env, ast_t* ast) {
  ast_t* right;
  ast_t* tmp;
  switch(ast->data.assignment.right->type) {
    case at_function:
      right = ast->data.assignment.right;
      break;
    case at_bool:
    case at_double:
    case at_integer:
    case at_string:
      right = ast->data.assignment.right;
      break;
    case at_identifier:
      right = (ast_t*)get_value(env->map, ast->data.assignment.right->data.id);
      break;
    case at_expression:
      right = eval_expression(env, ast->data.assignment.right);
      break;
    case at_call:
      // TODO: implementieren.
      break;
    default:
      break;

  }
  if(right == NULL) {
    error_cannot_assign(NULL,"NULL",ast->data.assignment.id);
  }
  tmp = add_value(env->map, ast->data.assignment.id, right);
}

/* prototypes in "vm.h" */

env_t* create_env(void) {
  env_t* env;
  env = (env_t*)check_malloc(sizeof(env_t));
  env->map = create_map(0);
  return env;
}

void free_env(env_t* env) {
  free_map(env->map, 0); /* no need to free elements */  // FIXME: thats not true! --> i need to free some of them (-> ref_count)
  free(env);
}

void vm_exec(env_t* env, ast_t* ast) {
  if(ast->type == at_statements) {
    exec_statements(env,ast);
  } else {
// TODO : -----
  }
}


