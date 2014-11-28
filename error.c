
/* lib */
#include <stdio.h>

/* own */
#include "ast.h"

/* prototypes */
void error(position_t* pos, char* emsg);

/* prototypes in "error.c" */
void error(position_t* pos, char* emsg) {
  if(pos == NULL) {
    fprintf(stderr, "error: %s\n",emsg);
  } else {
    fprintf(stderr,"error(%s:%d): %s\n", pos->file, pos->line, emsg);
  }
  exit(1);
}

/* prototypes in "error.h" */
void error_expected(position_t* pos, char* expected, char* received) {
  char buf[250];
  sprintf(buf,"expected: \"%s\", received: \"%s\"", expected, received);
  error(buf);
}

void error_cannot_assign(position_t* pos, char* from, char* to) {
  char buf[250];
  sprintf(buf,"cannot assign \"%s\" to \"%s\"", from, to);
  error(buf);
}
