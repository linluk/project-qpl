#ifndef __ERROR_H__
#define __ERROR_H__

#include "ast.h"

void error_expected(position_t* pos, const char* expected, const char* received);

void error_cannot_assign(position_t* pos, const char* from, const char* to);

#endif /* __ERROR_H__ */
