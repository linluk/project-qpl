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

%option noyywrap
%option nounput
%option noinput
%option outfile="lexer.c" header-file="lexer.h"

%{
/* lib */
#include <stdlib.h>
#include <inttypes.h>

/* own */
#include "ast.h"
#include "parser.h"
#include "utils.h"
#include "error.h"

void lex_integer(char* value);
void lex_double(char* value);
void lex_string(char* value);
void lex_bool(char* value);
void lex_operator(char* value);
void lex_id(char* value);
void lex_error(void);
void lex_inc_line(void);
%}


%x X_COMMENT

%%


"//".*                       { /* skip */ }
"/*"                         { BEGIN(X_COMMENT); }
<X_COMMENT>{
"*/"                         { BEGIN(INITIAL); }
[^*\n]+                      { /* skip */ }
"*"                          { /* skip */ }
\n                           { lex_inc_line(); }
}
[ \t]                        { /* skip white */ }
\(                           { return T_LPAREN; }
\)                           { return T_RPAREN; }
\{                           { return T_LBRACKET; }
\}                           { return T_RBRACKET; }
\=                           { return T_ASSIGN; }
"\n"                         { lex_inc_line(); }
";"                          { return T_DELIMITER; }
","                          { return T_COMMA; }
"if"                         { return T_IF; }
"elif"                       { return T_ELIF; }
"else"                       { return T_ELSE; }
"while"                      { return T_WHILE; }
"do"                         { return T_DO; }
"true"|"false"               { lex_bool(yytext); return T_BOOL; }
[a-zA-Z_][a-zA-Z0-9_]*       { lex_id(yytext); return T_ID; }
@                            { lex_id(yytext); return T_AT; }
(0|[1-9][0-9]*)              { lex_integer(yytext); return T_INTEGER; }
(0|[1-9][0-9]*)\.[0-9]+      { lex_double(yytext); return T_DOUBLE; }
\"[^\"\n]*\"                 { lex_string(yytext); return T_STRING; }
"+"|"-"                      { lex_operator(yytext); return T_ADDOP; }
"*"|"/"|"%"                  { lex_operator(yytext); return T_MULOP; }
">"|"<"|">="|"<="|"=="|"!="  { lex_operator(yytext); return T_CMPOP; }
"&"                          { lex_operator(yytext); return T_ANDOP; }
"|"                          { lex_operator(yytext); return T_OROP; }
"$"                          { lex_operator(yytext); return T_STROP; }
.                            { lex_error(); }

%%

void lex_integer(char* value) {
  yylval.i = strtoimax(value,NULL,10);
}

void lex_double(char* value) {
  yylval.d = atof(value);
}

void lex_string(char* value) {
  yylval.s = replace_str(yytext,"\"",""); // trim the delimiting quotes
}

void lex_bool(char* value) {
  if(strcmp(value,"true") == 0) {
    yylval.b = 1;
  } else if(strcmp(value,"false") == 0) {
    yylval.b = 0;
  } else {
    position_t pos;
    pos.file = NULL;
    pos.line = yylineno;
    error_unexpected(&pos,value);
  }
}

void lex_operator(char* value) {
#define OP(x)  {yylval.o = (x);}
#define OPB(x) {yylval.o = (x); break;}
  if(strlen(value) == 1) {
    switch(value[0]) {
      case '+': OPB(op_add);
      case '-': OPB(op_sub);
      case '*': OPB(op_mul);
      case '/': OPB(op_div);
      case '%': OPB(op_mod);
      case '>': OPB(op_gt);
      case '<': OPB(op_lt);
      case '&': OPB(op_and);
      case '|': OPB(op_or);
      case '$': OPB(op_cat);
      default:{
        position_t p;
        p.file = NULL;
        p.line = yylineno;
        error_unexpected(&p,value);
      }
    }
  } else {
    /* */ if(strcmp(value,"!=") == 0) {
      OP(op_neq);
    }else if(strcmp(value,"==") == 0) {
      OP(op_eq);
    }else if(strcmp(value,">=") == 0) {
      OP(op_ge);
    }else if(strcmp(value,"<=") == 0) {
      OP(op_le);
    } else {
      position_t p;
      p.file = NULL;
      p.line = yylineno;
      error_unexpected(&p,value);
    }
  }
#undef OP
#undef OPB
}

void lex_id(char* value) {
  yylval.id = strdup(value);
}

void lex_error(void) {
  position_t p;
  p.file = NULL;
  p.line = yylineno;
  error_unexpected(&p,"unknown token");
}

void lex_inc_line(void) {
  yylineno++;
}


