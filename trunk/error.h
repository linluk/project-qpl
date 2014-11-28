#ifndef __ERROR_H__
#define __ERROR_H__

#include "ast.h"

void error_expected(position_t* pos, char* expected, char* received);

void error_cannot_assign(position_t* pos, char* from, char* to);

#endif /* __ERROR_H__ */
