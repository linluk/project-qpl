/*****************************************************************************
 *
 * Copyright (C) 2014 - 2016 Lukas Singer
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
 * Source code can be found under: <https://github.com/linluk/project-qpl/>.
 *
 ****************************************************************************/

#include <stdio.h>

/* needed to create a stack trace on crash */
#include <execinfo.h>
#include <signal.h>

#include "ast.h"
#include "vm.h"
#include "env.h"
#include "builtin.h"
#include "utils.h"

int yyparse(ast_t** ast_dest); /* get rid of implicit declaration warning */
extern int yydebug;
#include "lexer.h"
#include "parser.h"


void sigsegv_handler(int sig) {
  void *array[25];
  size_t size;
  // get void*'s for all entries on the stack
  size = backtrace(array, 25);
  // print out all the frames to stderr
  fprintf(stderr, "Error: signal %d:\n", sig);
  backtrace_symbols_fd(array, size, STDERR_FILENO);
  exit(1);
}


int main(int argc, char** argv) {
  signal(SIGSEGV, sigsegv_handler);   // install our sigsegv_handler
  // disable in production code

  ast_t* ast = NULL;

  if(argc > 1) {
    yyin = fopen(argv[1], "r");
  } else {
    yyin = stdin;
    printf("type your code and press <CTRL> + <D> \n");
  }

  //  yydebug = 1;

  if(yyin == NULL) {
    printf("yyin == NULL\n");
  }

  yyparse(&ast);

  //  print_ast(ast,0);

  if (yyin != stdin) {
    fclose(yyin);
  }

  if(ast != NULL) {

    env_t* env = create_env();
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

