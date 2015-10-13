/*****************************************************************************
 *
 * Copyright (C) 2014 - 2015 Lukas Singer
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

%error-verbose
%output "parser.c"
%defines "parser.h"
%{

/* lib */
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

/* own */
#include "ast.h"

/* created by 'flex' */
#include "lexer.h"

#define YYDEBUG (1)

%}

%parse-param { ast_t** ast_dest }

/* this union defines the yyval structure -> the $$, $1..$N values */
%union {
  intmax_t i;        /* integer */
  long double d;     /* double */
  char b;            /* bool */
  char* s;           /* string */
  enum operator_e o; /* operator */
  char* id;          /* identifier */
  struct ast_s* ast; /* abstract syntax tree */
  /* ast_t doesnt work!
  * i have to use "struct ast_s" instead!
  * why? astgen.h is included above!
  */
}

%token T_LBRACKET T_RBRACKET T_LPAREN T_RPAREN T_LSQBRACKET T_RSQBRACKET T_COLON T_ASSIGN T_WHILE T_COMMA T_DELIMITER T_DO
%token<id> T_ID T_AT
%token<o> T_ADDOP T_MULOP T_CMPOP T_ANDOP T_OROP T_STROP T_IF T_ELIF T_ELSE
%token<i> T_INTEGER
%token<d> T_DOUBLE
%token<s> T_STRING
%token<b> T_BOOL
%type<ast> statement statements assignment expression term factor conditional if_statement elif_statement elif_statements else_statement loop while_loop do_while_loop block call callargs non_empty_callargs function params non_empty_params list list_values
%type<id> assignleft

%start program

%{

void yyerror(ast_t** ast, const char* const emsg);

%}

%%

program : statements { (*(ast_t**)ast_dest) = $1; }
        ;

statements :  { $$ = NULL; }
           | statements statement { $$ = create_statement($1, $2); }
           ;

statement : assignment T_DELIMITER { $$ = $1; }
          | conditional { $$ = $1; }
          | loop { $$ = $1; }
          | call T_DELIMITER { $$ = $1; }
          ;

assignment : assignleft T_ASSIGN expression { $$ = create_assignment($1, $3); }
           | assignleft T_ASSIGN function { $$ = create_assignment($1, $3); }
           ;

assignleft : T_ID { $$ = $1; }
           | T_AT { $$ = $1; }
           ;

expression : term T_CMPOP expression { $$ = create_expression($2, $1, $3); }
           | term T_STROP expression { $$ = create_expression($2, $1, $3); }
           | term T_OROP expression { $$ = create_expression($2, $1, $3); }
           | term T_ADDOP expression { $$ = create_expression($2, $1, $3); }
           | term { $$ = $1; }
           /*| list { $$ = $1; }*/
           ;

term : term T_ANDOP factor { $$ = create_expression($2, $1, $3); }
     | term T_MULOP factor { $$ = create_expression($2, $1, $3); }
     | factor { $$ = $1; }
     ;

factor : T_INTEGER { $$ = create_integer($1); }
       | T_DOUBLE { $$ = create_double($1); }
       | T_BOOL { $$ = create_bool($1); }
       | T_STRING { $$ = create_string($1); }
       | T_ID { $$ = create_identifier($1); }
       | call { $$ = $1; }
       | T_LPAREN expression T_RPAREN { $$ = $2; }
       | list { $$ = $1; }
       | factor T_LSQBRACKET expression T_RSQBRACKET { $$ = create_expression(op_deref, $1, $3); }
       ;

conditional : if_statement elif_statements else_statement { $$ = create_conditional($1, $2, $3); }
            ;

if_statement : T_IF T_LPAREN expression T_RPAREN block { $$ = create_if($3, $5); }
             ;

elif_statement : T_ELIF T_LPAREN expression T_RPAREN block { $$ = create_if($3, $5);/* create_if() here and combine them to elif_statement_S_ */ }
               ;

elif_statements :  { $$ = NULL; }
                | elif_statements elif_statement { $$ = create_elif($1, $2); }
                ;

else_statement :  { $$ = NULL; }
               | T_ELSE block { $$ = $2; }
               ;

loop : while_loop { $$ = $1; }
     | do_while_loop { $$ = $1; }
     ;

while_loop : T_WHILE T_LPAREN expression T_RPAREN block { $$ = create_while($3, $5); }
           ;

do_while_loop : T_DO block T_LPAREN expression T_RPAREN { $$ = create_dowhile($4, $2); }
              ;

list : T_LSQBRACKET list_values T_RSQBRACKET { $$ = $2; }
     | T_LSQBRACKET T_RSQBRACKET { $$ = NULL; }
     ;

list_values : expression { $$ = create_list(NULL, $1); }
            | list_values T_COMMA expression { $$ = create_list($1, $3); }
            ;

block : T_LBRACKET statements T_RBRACKET { $$ = $2; }
      ;

call : T_ID T_LPAREN callargs T_RPAREN { $$ = create_call($1, NULL, $3); }
     | function T_LPAREN callargs T_RPAREN { $$ = create_call(NULL, $1, $3); }
     ;

callargs : non_empty_callargs { $$ = $1; }
         |  { $$ = NULL; }
         ;

non_empty_callargs : expression { $$ = create_callarg(NULL, $1); }
                   | non_empty_callargs T_COMMA  expression { $$ = create_callarg($1, $3); }
                   ;

function : T_AT T_LPAREN params T_RPAREN block { $$ = create_function($3, $5); }
         ;

params : non_empty_params { $$ = $1; }
       |  { $$ = NULL; }
       ;

non_empty_params : T_ID { $$ = create_param(NULL, $1); }
                 | non_empty_params T_COMMA T_ID { $$ = create_param($1, $3); }
                 ;

%%

void yyerror(ast_t** ast, const char* const emsg) {
  fprintf(stderr, "parse error (line %d): \"%s\"\n", yylineno, emsg);
  exit(1);
}

