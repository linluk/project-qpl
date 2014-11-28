/* lib */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* own */
#include "utils.h"

/* self */
#include "ast.h"

/* ast type names, these are returned by get_ast_type_name() */
static const char* at_identifier_str = "identifier";
static const char* at_integer_str = "integer";
static const char* at_double_str = "double";
static const char* at_string_str = "string";
static const char* at_bool_str = "bool";
static const char* at_expression_str = "expression";
static const char* at_assignment_str = "assignment";
static const char* at_call_str = "call";
static const char* at_callargs_str = "callargs";
static const char* at_statements_str = "statements";
static const char* at_conditional_str = "conditional";
static const char* at_if_str = "if";
static const char* at_elif_str = "elif";
static const char* at_while_str = "while";
static const char* at_dowhile_str = "do-while";
static const char* at_function_str = "function";
static const char* at_params_str = "params";
static const char* at_end_str = "end";
static const char* at_unknown_str = "unknown";

/* prototypes */
ast_t* create_ast(ast_type_t type);

/* prototypes in "astgen.c" */
ast_t* create_ast(ast_type_t type) {
  ast_t* ast;
  ast = check_malloc(sizeof(ast_t));
  ast->type = type;
  ast->ref_count = -1; /* created by parser --> never call free! */
  return ast;
}

/* prototypes in "astgen.h" */
ast_t* create_integer(int value) {
  ast_t* ast;
  ast = create_ast(at_integer);
  ast->data.i = value;
  return ast;  
}

ast_t* create_double(double value) {
  ast_t* ast;
  ast = create_ast(at_double);
  ast->data.d = value;
  return ast;
}

ast_t* create_bool(char value) {
  ast_t* ast;
  ast = create_ast(at_bool);
  ast->data.b = value;
  return ast;
}

ast_t* create_string(char* value) {
  ast_t* ast;
  ast = create_ast(at_string);
  ast->data.s = value;
  return ast;
}

ast_t* create_identifier(char* id) {
  ast_t* ast;
  ast = create_ast(at_identifier);
  ast->data.id = id;
  return ast;
}

ast_t* create_expression(operator_t op, ast_t* left, ast_t* right) {
  ast_t* ast;
  ast = create_ast(at_expression);
  ast->data.expression.op = op;
  ast->data.expression.left = left;
  ast->data.expression.right = right;
  return ast;
}

ast_t* create_assignment(char* id, ast_t* right) {
  ast_t* ast;
  ast = create_ast(at_assignment);
  ast->data.assignment.id = id;
  ast->data.assignment.right = right;
  return ast;
}
ast_t* create_statement(ast_t* statements, ast_t* statement) {
  if(statements == NULL) {
    statements = create_ast(at_statements);
    statements->data.statements.count = 0;
    statements->data.statements.statements = NULL;
  }
  if(statements->type != at_statements) {
    fprintf(stderr, "ast generation error\n");
    exit(1);
  }
  statements->data.statements.count++;
  statements->data.statements.statements = (ast_t**)check_realloc(statements->data.statements.statements, statements->data.statements.count * sizeof(ast_t*));
  statements->data.statements.statements[statements->data.statements.count - 1] = statement;
  return statements;
}

ast_t* create_call(ast_t* id, ast_t* callargs) {
  ast_t* ast;
  ast = create_ast(at_call);
  ast->data.call.id = id;
  ast->data.call.callargs = callargs;
  return ast;
}

ast_t* create_callarg(ast_t* callargs, ast_t* callarg) {
  if(callargs == NULL) {
    callargs = create_ast(at_callargs);
    callargs->data.callargs.count = 0;
    callargs->data.callargs.callargs = NULL;
  }
  if(callargs->type != at_callargs) {
    fprintf(stderr, "ast generation error\n");
    exit(1);
  }
  callargs->data.callargs.count++;
  callargs->data.callargs.callargs = (ast_t**)check_realloc(callargs->data.callargs.callargs, callargs->data.callargs.count * sizeof(ast_t*));
  callargs->data.callargs.callargs[callargs->data.callargs.count - 1] = callarg;
  return callargs;
}

ast_t* create_conditional(ast_t* if_statement, ast_t* elif_statements, ast_t* else_statement) {
  ast_t* ast;
  ast = create_ast(at_conditional);
  ast->data.conditional.if_statement = if_statement;
  ast->data.conditional.elif_statements = elif_statements;
  ast->data.conditional.else_statement = else_statement;
  return ast;
}

ast_t* create_if(ast_t* condition, ast_t* statements) {
  ast_t* ast;
  ast = create_ast(at_if);
  ast->data.if_statement.condition = condition;
  ast->data.if_statement.statements = statements;
  return ast;
}

ast_t* create_elif(ast_t* elif_statements, ast_t* elif_statement) {
  if(elif_statements == NULL) {
    elif_statements = create_ast(at_elif);
    elif_statements->data.elif_statements.count = 0;
    elif_statements->data.elif_statements.elif_statements = NULL;
  }
  if(elif_statements->type != at_elif) {
    fprintf(stderr, "ast generation error\n");
    exit(1);
  }
  elif_statements->data.elif_statements.count++;
  elif_statements->data.elif_statements.elif_statements = (ast_t**)check_realloc(elif_statements->data.elif_statements.elif_statements, elif_statements->data.elif_statements.count * sizeof(ast_t*));
  elif_statements->data.elif_statements.elif_statements[elif_statements->data.elif_statements.count - 1] = elif_statement;
  return elif_statements;
}

ast_t* create_while(ast_t* condition, ast_t* statements) {
  ast_t* ast;
  ast = create_ast(at_while);
  ast->data.while_statement.condition = condition;
  ast->data.while_statement.statements = statements;
  return ast;
}

ast_t* create_dowhile(ast_t* condition, ast_t* statements) {
  ast_t* ast;
  ast = create_ast(at_dowhile);
  ast->data.dowhile_statement.condition = condition;
  ast->data.dowhile_statement.statements = statements;
  return ast;
}

ast_t* create_function(ast_t* params, ast_t* statements) {
  ast_t* ast;
  ast = create_ast(at_function);
  ast->data.function.params = params;
  ast->data.function.statements = statements;
  return ast;
}

ast_t* create_param(ast_t* params, ast_t* param) {
  if(params == NULL) {
    params = create_ast(at_params);
    params->data.params.count = 0;
    params->data.params.params = NULL;
  }
  if(params->type != at_params) {
    fprintf(stderr, "ast generation error\n");
    exit(1);
  }
  params->data.params.count++;
  params->data.params.params = (ast_t**)check_realloc(params->data.params.params, params->data.params.count * sizeof(ast_t*));
  params->data.params.params[params->data.params.count - 1] = param;
  return params;
}

const char* get_ast_type_name(ast_type_t ast) {
  switch(ast) {
    case at_assignment: return at_assignment_str;
    case at_bool: return at_bool_str;
    case at_call: return at_call_str;
    case at_identifier: return at_identifier_str;
    case at_callargs: return at_callargs_str;
    case at_conditional: return at_conditional_str;
    case at_double: return at_double_str;
    case at_dowhile: return at_dowhile_str;
    case at_elif: return at_elif_str;
    case at_end: return at_end_str;
    case at_expression: return at_expression_str;
    case at_function: return at_function_str;
    case at_if: return at_if_str;
    case at_integer: return at_integer_str;
    case at_params: return at_params_str;
    case at_statements: return at_statements_str;
    case at_string: return at_string_str;
    case at_while: return at_while_str;
    default: return at_unknown_str;
  }
}

void print_ast(ast_t* ast, int indent){
  size_t i;
  for(i = 0; i < indent; i++) {
    printf(" ");
  }
  if(ast != NULL) {
    switch(ast->type) {
      case at_identifier:
        printf("identifier: %s\n",ast->data.id);
        break;
      case at_integer:
        printf("integer: %d\n",ast->data.i);
        break;
      case at_double:
        printf("double: %f\n",ast->data.d);
        break;
      case at_string:
        printf("string: \"%s\"\n",ast->data.s);
        break;
      case at_bool:
        printf("bool: %s\n",ast->data.b > 0 ? "true" : "false");
        break;
      case at_expression:
        printf("expression: op%d\n",ast->data.expression.op);
        print_ast(ast->data.expression.left,indent+2);
        print_ast(ast->data.expression.right,indent+2);
        break;
      case at_assignment:
        printf("assignment: %s\n",ast->data.assignment.id);
        print_ast(ast->data.assignment.right,indent+2);
        break;
      case at_call:
        printf("call:\n");
        print_ast(ast->data.call.id,indent+2);
        print_ast(ast->data.call.callargs,indent+2);
        break;
      case at_callargs:
        printf("callargs:\n");
        for(i = 0; i < ast->data.callargs.count; i++) {
          print_ast(ast->data.callargs.callargs[i],indent+2);
        }
        break;
      case at_statements:
        printf("statements:\n");
        for(i = 0; i < ast->data.statements.count; i++) {
          print_ast(ast->data.statements.statements[i],indent+2);
        }
        break;
      case at_conditional:
        printf("conditional:\n");
        print_ast(ast->data.conditional.if_statement,indent+2);
        print_ast(ast->data.conditional.elif_statements,indent+2);
        print_ast(ast->data.conditional.else_statement,indent+2);
        break;
      case at_if:
        printf("if:\n");
        print_ast(ast->data.if_statement.condition,indent+2);
        print_ast(ast->data.if_statement.statements,indent+2);
        break;
      case at_elif:
        printf("elif: %d\n",ast->data.elif_statements.count);
        for(i = 0; i < ast->data.elif_statements.count; i++) {
          print_ast(ast->data.elif_statements.elif_statements[i],indent+2);
        }
        break;
      case at_while:
        printf("while:\n");
        print_ast(ast->data.while_statement.condition,indent+2);
        print_ast(ast->data.while_statement.statements,indent+2);
        break;
      case at_dowhile:
        printf("do-while:\n");
        print_ast(ast->data.dowhile_statement.condition,indent+2);
        print_ast(ast->data.dowhile_statement.statements,indent+2);
        break;
      case at_function:
        printf("function:\n");
        print_ast(ast->data.function.params,indent+2);
        print_ast(ast->data.function.statements,indent+2);
        break;
      case at_params:
        printf("params: %d ( ",ast->data.params.count);
        for(i = 0; i < ast->data.params.count; i++) {
          if(i != 0) {
            printf(", ");
          }
          print_ast(ast->data.params.params[i],indent+2);
        }
        printf(")\n");
        break;
      default:
        fprintf(stderr,"warning: unknown ast type: %d\n", ast->type);
        exit(1);
        break;
    }
  } else {
    printf("/* empty */\n");
  }
}

void free_ast(ast_t* ast) {
  size_t i;
  if(ast != NULL) {
    switch(ast->type) {
      case at_identifier:
        free(ast->data.id);
        break;
      case at_integer:
        break;
      case at_double:
        break;
      case at_string:
        free(ast->data.s);
        break;
      case at_bool:
        break;
      case at_expression:
        free_ast(ast->data.expression.left);
        free_ast(ast->data.expression.right);
        break;
      case at_assignment:
        free(ast->data.assignment.id);
        free_ast(ast->data.assignment.right);
        break;
        break;
      case at_call:
        free(ast->data.call.id);
        free_ast(ast->data.call.callargs);
        break;
      case at_callargs:
        for(i = 0; i < ast->data.callargs.count; i++) {
          free_ast(ast->data.callargs.callargs[i]);
        }
        break;
      case at_statements:
        for(i = 0; i < ast->data.statements.count; i++) {
          free_ast(ast->data.statements.statements[i]);
        }
        break;
      case at_conditional:
        free_ast(ast->data.conditional.if_statement);
        free_ast(ast->data.conditional.elif_statements);
        free_ast(ast->data.conditional.else_statement);
        break;
      case at_if:
        free_ast(ast->data.if_statement.condition);
        free_ast(ast->data.if_statement.statements);
        break;
      case at_elif:
        for(i = 0; i < ast->data.elif_statements.count; i++) {
          free_ast(ast->data.elif_statements.elif_statements[i]);
        }
        break;
      case at_while:
        free_ast(ast->data.while_statement.condition);
        free_ast(ast->data.while_statement.statements);
        break;
      case at_dowhile:
        free_ast(ast->data.dowhile_statement.condition);
        free_ast(ast->data.dowhile_statement.statements);
        break;
      case at_function:
        free_ast(ast->data.function.params);
        free_ast(ast->data.function.statements);
        break;
      case at_params:
        for(i = 0; i < ast->data.params.count; i++) {
          free(ast->data.params.params[i]);
        }
        break;
      default:
        break;
    }
    free(ast);
  }
}

