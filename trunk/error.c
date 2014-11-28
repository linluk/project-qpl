
/* lib */
#include <stdlib.h>
#include <stdio.h>

/* own */
#include "ast.h"

/* prototypes */
void error(position_t* pos, const char* emsg);

/* prototypes in "error.c" */
void error(position_t* pos, const char* emsg) {
  if(pos == NULL) {
    fprintf(stderr, "error: %s\n",emsg);
  } else {
    fprintf(stderr,"error(%s:%d): %s\n", pos->file, pos->line, emsg);
  }
  exit(1);
}

/* prototypes in "error.h" */
void error_expected(position_t* pos, const char* expected, const char* received) {
  char buf[250];
  sprintf(buf,"expected: \"%s\", received: \"%s\"", expected, received);
  error(pos,buf);
}

void error_assign(position_t* pos, const char* from, const char* to) {
  char buf[250];
  sprintf(buf,"cannot assign \"%s\" to \"%s\"", from, to);
  error(pos,buf);
}

void error_apply(position_t* pos, const char* op, const char* arg1, const char* arg2) {
  char buf[250];
  sprintf(buf,"cannot apply \"%s\" to \"%s\" and \"%s\"", op, arg1, arg2);
  error(pos,buf);
}
