#!/bin/bash

echo "compiling $1"
rm -f lex.yy.c
rm -f $1.tab.c
rm -f $1
flex $1.l
yacc -d $1.y
g++ *.c -o main