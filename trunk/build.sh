#!/bin/bash

# TODO: create a make file

# output on a "new" screen
clear

# i want to have english outputs (errors and warnings)
LANG=en

flex lexer.l
bison parser.y

gcc -g -Wall main.c ops.c env.c error.c vm.c ast.c utils.c map.c lexer.c parser.c -o qpl
