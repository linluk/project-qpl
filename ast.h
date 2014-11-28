#ifndef __AST_H__
#define __AST_H__

typedef enum ast_type_e {
  at_identifier,
  at_integer, at_double, at_string, at_bool,
  at_expression,
  at_assignment,
  at_call, at_callargs,
  at_statements,
  at_conditional,
  at_if, at_elif,
  at_while, at_dowhile,
  at_function, at_params,

  /* add values before this */
  at_end
} ast_type_t;

typedef enum operator_e {
  op_add, op_sub,
  op_mul, op_div, op_mod,
  op_lt, op_gt, op_le, op_ge,
  op_eq, op_neq,
  op_and, op_or,
  op_cat
} operator_t;

typedef struct position_s {
  char* file;
  int line;
} position_t;

/* THE abstract syntax tree structure */
typedef struct ast_s {
  ast_type_t type;
  int ref_count; /* positive value: ref_count -> call free when it becomes zero 
                    negative value: created by parser -> never call free */
  union { /* data */
    int i;
    double d;
    char b;
    char* s;
    char* id;
    struct { /* expression */
      operator_t op;
      struct ast_s* left;
      struct ast_s* right;
    } expression;
    struct { /* assignment */
      char* id;
      struct ast_s* right;
    } assignment;
    struct { /* statements */
      int count;
      struct ast_s** statements;
    } statements;
//    struct { /* loop */
//      struct ast_s* condition;
//      struct ast_s* statements;
//    } loop;
    struct { /* call */
      struct ast_s* id;
      struct ast_s* callargs;
    } call;
    struct { /* callargs */
      int count;
      struct ast_s** callargs;
    } callargs;
    struct { /* conditional */
      struct ast_s* if_statement;
      struct ast_s* elif_statements;
      struct ast_s* else_statement;
    } conditional;
    struct { /* if_statement */
      struct ast_s* condition;
      struct ast_s* statements;
    } if_statement;
    struct { /* elif_statements */
      int count;
      struct ast_s** elif_statements;
    } elif_statements;
    struct { /* while_statement */
      struct ast_s* condition;
      struct ast_s* statements;
    } while_statement;
    struct { /* dowhile_statement */
      struct ast_s* condition;
      struct ast_s* statements;
    } dowhile_statement;
    struct { /* function */
      struct ast_s* params;
      struct ast_s* statements;
    } function;
    struct { /* params */
      int count;
      struct ast_s** params;
    } params;
  } data;
} ast_t;

ast_t* create_integer(int value);
ast_t* create_double(double value);
ast_t* create_bool(char value);
ast_t* create_string(char* value);
ast_t* create_identifier(char* id);
ast_t* create_expression(operator_t op, ast_t* left, ast_t* right);
ast_t* create_assignment(char* id, ast_t* right);
ast_t* create_statement(ast_t* statements, ast_t* statement);
//ast_t* create_loop(ast_t* condition, ast_t* statements);
ast_t* create_call(ast_t* id, ast_t* callargs);
ast_t* create_callarg(ast_t* callargs, ast_t* callarg);
ast_t* create_conditional(ast_t* if_statement, ast_t* elif_statements, ast_t* else_statement);
ast_t* create_if(ast_t* condition, ast_t* statements);
ast_t* create_elif(ast_t* elif_statements, ast_t* elif_statement);
ast_t* create_while(ast_t* condition, ast_t* statements);
ast_t* create_dowhile(ast_t* condition, ast_t* statements);
ast_t* create_function(ast_t* params, ast_t* statements);
ast_t* create_param(ast_t* params, ast_t* id);

const char* get_ast_type_name(ast_type_t ast);
const char* get_op_str(operator_t op);

int is_numeric_ast_type(ast_type_t ast);

void print_ast(ast_t* ast, int indent);

void free_ast(ast_t* ast);

#endif /* __AST_H__ */
