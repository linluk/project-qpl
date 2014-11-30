/*****************************************************************************
 *
 * Copyright (C) 2014 Lukas Singer
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
 * Source code can be found under: <https://code.google.com/p/project-qpl/>.
 *
 ****************************************************************************/

#ifndef __ERROR_H__
#define __ERROR_H__

#include "ast.h"

void error_expected(position_t* pos, const char* expected, const char* received);
void error_assign(position_t* pos, const char* from, const char* to);
void error_apply(position_t* pos, const char* op, const char* arg1, const char* arg2);
void error_id(position_t* pos, const char* id);
void error_paramcount(position_t* pos, const char* func, int paramcount, int callargcount);

#endif /* __ERROR_H__ */
