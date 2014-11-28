%option noyywrap
%option outfile="lexer.c" header-file="lexer.h"

%{
/* lib */
#include <stdlib.h>

/* own */
#include "ast.h"
#include "parser.h"

void lex_integer(char* value);
void lex_double(char* value);
void lex_string(char* value);
void lex_bool(char* value);
void lex_operator(char* value);
void lex_id(char* value);
void lex_inc_line(void);

%}


%x X_COMMENT

%%


"/*"                                    { BEGIN(X_COMMENT); }
<X_COMMENT>{
"*/"                                    { BEGIN(INITIAL); }
[^*\n]+                                 { /* skip */ }
"*"                                     { /* skip */ }
\n                                      { lex_inc_line(); }
}
[ \t]                                   { /* skip white */ }
\(                                      { return T_LPAREN; }
\)                                      { return T_RPAREN; }
\{                                      { return T_LBRACKET; }
\}                                      { return T_RBRACKET; }
\=                                      { return T_ASSIGN; }
"\n"                                    { lex_inc_line(); }
;                                       { return T_DELIMITER; }
"if"                                    { return T_IF; }
"elif"                                  { return T_ELIF; }
"else"                                  { return T_ELSE; }
"while"                                 { return T_WHILE; }
"do"                                    { return T_DO; }
"true"|"false"                          { lex_bool(yytext); return T_BOOL; }
[a-zA-Z_][a-zA-Z0-9_]*                  { lex_id(yytext); return T_ID; }
@                                       { lex_id(yytext); return T_AT; }
(0|[1-9][0-9]*)                         { lex_integer(yytext); return T_INTEGER; }
(0|[1-9][0-9]*)\.[0-9]+                 { lex_double(yytext); return T_DOUBLE; }
\"[^\"\n]*\"                            { lex_string(yytext); return T_STRING; }
"+"|"-"                                 { lex_operator(yytext); return T_ADDOP; }
"*"|"/"|"%"                             { lex_operator(yytext); return T_MULOP; }
">"|"<"|">="|"<="|"=="|"!="             { lex_operator(yytext); return T_CMPOP; }
"&"                                     { lex_operator(yytext); return T_ANDOP; }
"|"                                     { lex_operator(yytext); return T_OROP; }
"$"                                     { lex_operator(yytext); return T_STROP; }
.                                       { fprintf(stderr, "unexpected token: \"%s\"", yytext); exit(1); }

%%

void lex_integer(char* value) {
  yylval.i = atoi(value);
}

void lex_double(char* value) {
  yylval.d = atof(value);
}

void lex_string(char* value) {
  yylval.s = strdup(yytext);
}

void lex_bool(char* value) {
  if(strcmp(value,"true") == 0) {
    yylval.b = 1;
  } else if(strcmp(value,"false") == 0) {
    yylval.b = 0;
  } else {
    fprintf(stderr,"unexpected literal: \"%s\"",value);
    exit(1);
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
      case '<': OPB(op_gt);
      case '&': OPB(op_and);
      case '|': OPB(op_or);
      case '$': OPB(op_cat);
      default:
        fprintf(stderr,"unexpected operator: \"%s\"",value);
        exit(1);
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
      fprintf(stderr,"unexpected operator: \"%s\"",value);
      exit(1);
    }
  }
}

void lex_id(char* value) {
  yylval.id = strdup(value);
}

void lex_inc_line(void) {
  yylineno++;
}

