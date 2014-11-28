#ifndef __VM_H__
#define __VM_H__

#include "map.h"
#include "ast.h"

typedef struct env_s {
  map_t* map;
} env_t;

env_t* create_env(void);
void free_env(env_t* env);

void vm_exec(env_t* env, ast_t* ast);

#endif /* __VM_H__ */
