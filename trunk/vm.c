
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

}

ast_t* eval_expression(env_t* env, ast_t* ast) {
  switch(ast->type) {
    /* valid */
    case at_bool: return ast;
    case at_call: return eval_call(env,ast);
    case at_identifier: return get_ast_by_id(env, ast->data.id);
    case at_double: return ast;
    case at_expression: 
      switch(ast->data.expression.op) {
        case op_add: return eval_add(env, ast->data.expression.left, ast->data.expression.right);
        case op_and: return eval_and(env, ast->data.expression.left, ast->data.expression.right);
        default: // TODO other operator evals << default only caous of warnings.
          break;
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
      right = get_ast_by_id(env, ast->data.assignment.right->data.id);
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


