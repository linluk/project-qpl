#ifndef __ERROR_H__
#define __ERROR_H__

#include "ast.h"

void error_expected(position_t* pos, const char* expected, const char* received);
void error_assign(position_t* pos, const char* from, const char* to);
void error_apply(position_t* pos, const char* op, const char* arg1, const char* arg2);

#endif /* __ERROR_H__ */
