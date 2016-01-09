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

/* lib */
#include <stdlib.h>
#include <stdio.h>

/* own */
#include "ast.h"

#define ERROR_BUFFER_LENGTH (255)

/* prototypes */
void error(position_t* pos, const char* emsg);

/* prototypes in "error.c" */
void error(position_t* pos, const char* emsg) {
  if(pos == NULL) {
    fprintf(stderr, "error: %s\n",emsg);
  } else {
    if(pos->file == NULL) {
      fprintf(stderr,"error (line: %d): %s\n", pos->line, emsg);
    } else {
      fprintf(stderr,"error (%s:%d): %s\n", pos->file, pos->line, emsg);
    }
  }
  exit(1);
}

/* prototypes in "error.h" */
void error_expected(position_t* pos, const char* expected, const char* received) {
  char buf[ERROR_BUFFER_LENGTH];
  sprintf(buf,"expected: \"%s\", received: \"%s\"", expected, received);
  error(pos,buf);
}

void error_unexpected(position_t* pos, const char* unexpected) {
  char buf[ERROR_BUFFER_LENGTH];
  sprintf(buf,"unexpected \"%s\"", unexpected);
  error(pos,buf);
}

void error_assign(position_t* pos, const char* from, const char* to) {
  char buf[ERROR_BUFFER_LENGTH];
  sprintf(buf,"cannot assign \"%s\" to \"%s\"", from, to);
  error(pos,buf);
}

void error_apply(position_t* pos, const char* op, const char* arg1, const char* arg2) {
  char buf[ERROR_BUFFER_LENGTH];
  sprintf(buf,"cannot apply \"%s\" to \"%s\" and \"%s\"", op, arg1, arg2);
  error(pos,buf);
}

void error_id(position_t* pos, const char* id) {
  char buf[ERROR_BUFFER_LENGTH];
  sprintf(buf,"identifier \"%s\" not found", id);
  error(pos,buf);
}

void error_paramcount(position_t* pos, const char* func, int paramcount, int callargcount) {
  char buf[ERROR_BUFFER_LENGTH];
  sprintf(buf,"function \"%s\" expects %d arguments, called with %d", func, paramcount, callargcount);
  error(pos,buf);
}

void error_unsupported(position_t* pos, const char* unsupported) {
  char buf[ERROR_BUFFER_LENGTH];
  sprintf(buf,"unsupported feature: \"%s\"",unsupported);
  error(pos,buf);
}

void error_convert(position_t* pos, const char* from, const char* to) {
  char buf[ERROR_BUFFER_LENGTH];
  sprintf(buf,"cannot convert \"%s\" into \"%s\"",from,to);
  error(pos,buf);
}

void error_astgen(position_t* pos, const char* err) {
  char buf[ERROR_BUFFER_LENGTH];
  if (err != NULL) {
    sprintf(buf,"AST create error: \"%s\"", err);
  } else {
    sprintf(buf,"AST create error");
  }
  error(pos, buf);
}

void error_failed(position_t* pos, const char* what) {
  char buf[ERROR_BUFFER_LENGTH];
  sprintf(buf, "%s failed", what);
  error(pos, buf);
}

