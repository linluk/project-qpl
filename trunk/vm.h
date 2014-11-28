#ifndef __VM_H__
#define __VM_H__

#include "map.h"
#include "ast.h"
#include "env.h"

void vm_exec(env_t* env, ast_t* ast);

void exec_statements(env_t* env, ast_t* ast);
void exec_assignment(env_t* env, ast_t* ast);

ast_t* eval_call(env_t* env, ast_t* ast);
ast_t* eval_expression(env_t* env, ast_t* ast);

#endif /* __VM_H__ */
