
/* lib */
#include <stdlib.h>

/* own */
#include "utils.h"
#include "map.h"

/* self */
#include "env.h"

env_t* create_env(void) {
  env_t* env;
  env = (env_t*)check_malloc(sizeof(env_t));
  env->map = create_map(0);
  env->parent = NULL;
  return env;
}

void free_env(env_t* env) {
  free_map(env->map, 0); /* no need to free elements */  // FIXME: thats not true! --> i need to free some of them (-> ref_count)
  free(env);
}

ast_t* get_ast_by_id(env_t* env, char* id) {
  ast_t* ast;
  ast = get_value(env->map, id);
  if(ast == NULL && env->parent != NULL) {
    ast = get_ast_by_id(env->parent, id);
  }
  return ast;
}

void set_ast_to_id(env_t* env, char* id, ast_t* ast) {
  ast_t* old;
  old = add_value(env->map, id, ast);
  if(old != NULL) {
    if(old->ref_count > 0) {
      old->ref_count--;
      if(old->ref_count == 0) { 
        free_ast(old);
      }
    }
  }
}

