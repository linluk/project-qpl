#
# Copyright (C) 2014 - 2016 Lukas Singer
#
# This file is part of 'qpl'.
#
# 'qpl' is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# 'qpl' is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with 'qpl'.  If not, see <http://www.gnu.org/licenses/>.
#
# The name 'qpl' stands for Quick Programming Language and is a working
# title.  It may changes in the future.
#
# Source code can be found under: <https://github.com/linluk/project-qpl/>.
#

LEX=flex
YACC=bison
CC=gcc
#-rdynamic is needed for stack trace on crash
CFLAGS=-g -rdynamic -Wall
LFLAGS=-g -rdynamic
BIN=qpl
OBJ=parser.o lexer.o main.o ops.o env.o error.o vm.o ast.o utils.o map.o \
    builtin.o


BIN: $(OBJ)
	$(CC) $(LFLAGS) $(OBJ) -o $(BIN)

%.o: %.c parser.c lexer.c
	$(CC) -c $(CFLAGS) $<

parser.c: parser.y
	$(YACC) $<

lexer.c: lexer.l
	$(LEX) $<

clean:
	rm -f $(BIN) $(OBJ) lexer.c lexer.h parser.c parser.h parser.output

