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

#include <stdio.h>


#include "ast.h"
#include "vm.h"
#include "env.h"
#include "builtin.h"
#include "utils.h"

int yyparse(ast_t** ast_dest); /* get rid of implicit declaration warning */
#include "lexer.h"
#include "parser.h"


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

//  print_ast(ast,0);

  fclose(yyin);

  if(ast != NULL) {
    
//    printf("ast != NULL\n");

    env_t* env;
    env = create_env();
    populate_env(env);

    vm_exec(env, ast);

//    free_ast(ast); // free ast didnt work, because vm_exec() restructures the ast
                     // therefor it could be that there are more than one pointers
                     // pointing to the same memory. then when i free the ast
                     // every pointer get freed --> double free memory --> ggrrrrr!
                     //  WHAT IS MY SOLUTION ? ? ? ?
                     //
                     //  is this really the problem?? think about it again !!
                     //  does vm_exec() really changes the ast?? that would be
                     //  dangerous at all: think about a loop or a recursive function.
    free_env(env);

  } else {
    printf("ast == NULL\n");
    return 1;
  }
  return 0;
}
