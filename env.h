#ifndef __ENV_H__
#define __ENV_H__

#include "map.h"
#include "ast.h"

typedef struct env_s {
  struct env_s* parent;
  map_t* map;
} env_t;

env_t* create_env(void);

void free_env(env_t* env);

ast_t* get_ast_by_id(env_t* env, char* id);

void set_ast_to_id(env_t* env, char* id, ast_t* ast);


#endif /* __ENV_H__ */
