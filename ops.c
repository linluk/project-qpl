
/* lib */
#include <stdio.h>

/* own */
#include "ast.h"
#include "vm.h"
#include "env.h"
#include "error.h"

/* self */
#include "ops.h"

ast_t* eval_add(env_t* env, ast_t* ast1, ast_t* ast2) {
  ast_t* a1;
  ast_t* a2;
  ast_t* result;
  a1 = eval_expression(env,ast1);
  a2 = eval_expression(env,ast2);
  switch(a1->type) {
    case at_integer:
      switch(a2->type) {
        case at_integer:
          result = create_integer(a1->data.i + a2->data.i);
          break;
        case at_double:
          result = create_double(a1->data.i + a2->data.d);
          break;
        default:
          error_apply(NULL,get_op_str(op_add), get_ast_type_name(a1->type), get_ast_type_name(a2->type));
          break;
      }
      break;
    case at_double:
      switch(a2->type) {
        case at_integer:
          result = create_double(a1->data.d + a2->data.i);
          break;
        case at_double:
          result = create_double(a1->data.d + a2->data.d);
        default:
          error_apply(NULL,get_op_str(op_add), get_ast_type_name(a1->type), get_ast_type_name(a2->type));
          break;
      }
      break;
    default:
      error_apply(NULL,get_op_str(op_add), get_ast_type_name(a1->type), get_ast_type_name(a2->type));
      break;
  }
  result->ref_count++;
  if(a1->ref_count == 0) { free_ast(a1); }
  if(a2->ref_count == 0) { free_ast(a2); }
  return result;
}

ast_t* eval_and(env_t* env, ast_t* ast1, ast_t* ast2) {

}


