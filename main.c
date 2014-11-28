#include <stdio.h>


#include "ast.h"
#include "vm.h"

#include "lexer.h"
#include "parser.h"

int yyparse(ast_t** ast_dest); /* get rid of implicit declaration warning */

int main(int argc, char** argv) {

  ast_t* ast;
  ast = NULL;

/*  if(argc > 1) {
    yyin = fopen(argv[1], "r");
  } else {
    yyin = stdin;
  }
*/
  /*** testing ***/
  yyin = fopen("example.qpl", "r");

  if(yyin == NULL) {
    printf("yyin == NULL\n");
  }

  yyparse(&ast);

  print_ast(ast,0);

  fclose(yyin);

  if(ast != NULL) {
    
    printf("ast != NULL\n");

    env_t* env;
    env = create_env();

    vm_exec(env, ast);

    free_ast(ast);
    free_env(env);

  } else {
    printf("ast == NULL\n");
    return 1;
  }
  return 0;
}
