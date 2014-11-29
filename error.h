#ifndef __ERROR_H__
#define __ERROR_H__

#include "ast.h"

void error_expected(position_t* pos, const char* expected, const char* received);
void error_assign(position_t* pos, const char* from, const char* to);
void error_apply(position_t* pos, const char* op, const char* arg1, const char* arg2);
void error_id(position_t* pos, const char* id);
void error_paramcount(position_t* pos, const char* func, int paramcount, int callargcount);

#endif /* __ERROR_H__ */
