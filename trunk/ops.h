#ifndef __OPS_H__
#define __OPS_H__

#include "env.h"
#include "ast.h"

ast_t* eval_add(env_t* env, ast_t* ast1, ast_t* ast2);

ast_t* eval_and(env_t* env, ast_t* ast1, ast_t* ast2);




#endif /* __OPS_H__ */
