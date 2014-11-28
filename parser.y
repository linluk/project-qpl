%error-verbose
%output "parser.c"
%defines "parser.h"
%{

/* lib */
#include <stdlib.h>
#include <stdio.h>

/* own */
#include "ast.h"

#include "lexer.h"

#define YYDEBUG (1)

%}

%parse-param { ast_t** ast_dest }

/* this union defines the yyval structure -> the $$, $1..$N values */
%union {
  int i;             /* integer */
  double d;          /* double */
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

%token T_LBRACKET T_RBRACKET T_LPAREN T_RPAREN T_ASSIGN T_AT T_WHILE T_DO T_COMMA T_DELIMITER
%token<id> T_ID
%token<o> T_ADDOP T_MULOP T_CMPOP T_ANDOP T_OROP T_STROP T_IF T_ELIF T_ELSE
%token<i> T_INTEGER
%token<d> T_DOUBLE
%token<s> T_STRING
%token<b> T_BOOL
%type<ast> statement statements assignment expression term factor conditional if_statement elif_statement elif_statements else_statement loop while_loop do_while_loop block call callargs non_empty_callargs function params non_empty_params

%start program

%{

void yyerror(ast_t** ast, const char* const emsg);

%}

%%

program : statements { (*(ast_t**)ast_dest) = $1; }
        ;

statements :   { $$ = NULL; }
           | statements statement { $$ = create_statement($1, $2); }
           ;

statement : assignment T_DELIMITER { $$ = $1; }
          | conditional { $$ = $1; }
          | loop { $$ = $1; }
          | call T_DELIMITER { $$ = $1; }
          ;

assignment : T_ID T_ASSIGN expression { $$ = create_assignment($1, $3); /* variable */ }
           | T_ID T_ASSIGN function { $$ = create_assignment($1, $3); /* function */ }
           ;

expression : term T_CMPOP expression { $$ = create_expression($2, $1, $3); }
           | term T_STROP expression { $$ = create_expression($2, $1, $3); }
           | term T_OROP expression { $$ = create_expression($2, $1, $3); }
           | term T_ADDOP expression { $$ = create_expression($2, $1, $3); }
           | term { $$ = $1; }
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
       ;

conditional : if_statement elif_statements else_statement { $$ = create_conditional($1, $2, $3); }
            ;

if_statement : T_IF T_LPAREN expression T_RPAREN block { $$ = create_if($3, $5); }
             ;

elif_statement : T_ELIF T_LPAREN expression T_RPAREN block { $$ = create_if($3, $5);/* create_if() here an combine them to elif_statement_S_ */ }
               ;

elif_statements : { $$ = NULL; }
                | elif_statements elif_statement { $$ = create_elif($1, $2); }
                ;

else_statement : { $$ = NULL; }
               | T_ELSE block { $$ = $2; }
               ;

loop : while_loop { $$ = $1; }
     | do_while_loop { $$ = $1; }
     ;

while_loop : T_WHILE T_LPAREN expression T_RPAREN block { $$ = create_while($3, $5); }
           ;

do_while_loop : T_DO block T_LPAREN expression T_RPAREN { $$ = create_dowhile($4, $2); }
             ;

block : T_LBRACKET statements T_RBRACKET { $$ = $2; }
      ;

call : T_ID T_LPAREN callargs T_RPAREN { $$ = create_call(create_identifier($1), $3); }
     | function T_LPAREN callargs T_RPAREN { $$ = create_call($1, $3); }
     ;

callargs : non_empty_callargs { $$ = $1; }
         | { $$ = NULL; }
         ;

non_empty_callargs : expression { $$ = $1; }
                  | non_empty_callargs T_COMMA expression { $$ = create_callarg($1, $3); }
                  ;

function : T_AT T_LPAREN params T_RPAREN block { $$ = create_function($3, $5); }
         ;

params : non_empty_params { $$ = $1; }
       | { $$ = NULL; }
       ;

non_empty_params : T_ID { $$ = create_identifier($1); }
                 | non_empty_params T_COMMA T_ID { $$ = create_param($1, create_identifier($3)); }
                 ;

%%

void yyerror(ast_t** ast, const char* const emsg) {
  fprintf(stderr, "parse error: \"%s\"\n", emsg);
  exit(1);
}

