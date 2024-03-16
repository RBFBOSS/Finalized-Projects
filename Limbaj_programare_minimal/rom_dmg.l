%{
#include <stdio.h>
#include <iostream>
#include "y.tab.h"
%}

%option noyywrap
%%

"USER_CLASSES" { return USER_CLASSES; }
"USER_VARIABLES" { return USER_VARIABLES; }
"USER_FUNCTIONS" { return USER_FUNCTIONS; }
"program_start" {return PROGRAM_START; }
"program_end" {return PROGRAM_END; }

"daca" {return  IF; }
"altfel" {return ELSE; }
"cat_timp" {return WHILE; }
"pentru" {return FOR; }
"in_raza" {return IN_RANGE; }

"eg"|"neg"|"mmreg"|"mmceg"|"mmr"|"mmc" { return OPERATOR; }
"si" { return AND; }
"nu" { return NOT; }
"sau" { return OR; }

"plus" { return PLUS; }
"minus" { return MINUS; }
"ori" { return TIMES; }
"imp" { return DIV; }
"mod" { return MOD; }

"ADEVARAT" { return BOOL_TRUE; }
"FALS" {return BOOL_FALSE; }

"bool"|"int"|"float"|"char"|"string" { yylval.string = strdup(yytext); return VARIABLE_TYPE; }

"citire" { return KEY_INPUT; }
"afisare" { return KEY_OUTPUT; }

"func" { return FUNCTION; }
"returnare" { return RETURN; }

"clasa" { return CLASS; }
"acest" { return THIS; }
"->" { return ARROW; }

"egal" { return ASSIGN; }

"{" { return START_BRACE; }
"}" { return END_BRACE; }   

"[" { return START_SQUARE_BRACKET; }
"]" { return END_SQUARE_BRACKET; }

"(" { return START_ROUND_BRACKET; }
")" { return END_ROUND_BRACKET; }

";" { return SEMICOLON; }
"," { return COMMA; }

([0-9]|[1-9][0-9]*)\.[0-9]* {yylval.string = strdup(yytext); return ID;}
[a-z_A-Z][a-z_A-Z0-9]* {yylval.string = strdup(yytext); return ID;}
\".*\" { yylval.string = strdup(yytext); return STRING; }
[0-9]|[1-9][0-9]* { yylval.number = atoi(yytext); return NUMBER; } 

[ \t]+ ;
"\r"|"\n"|"\r\n" { yylineno++; }
. { return 0; }
%%