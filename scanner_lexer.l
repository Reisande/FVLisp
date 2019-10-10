%{
#include "scanner_lexer.h"
#include "FVLisp_grammar.tab.h"
%}

%option noyywrap

%%

[0-9]+					            yylval.digit = atoi(yytext); return NUM;
\"[a-zA-z][a-zA-Z0-9]*\" 	  yylval.name = yytext; return STR;
(#t|#f) 				            yylval.bool = strcmp((yytext), "#t") == 0 ? 1 : 0; return BOOL;
"+"						              return ADD;
"-"						              return SUB;
"*"						              return MUL;
"/"						              return DIV;
^def$					              return DEF;
^if$   					            return IF; 
"'("					              return LISTL; 
"("                         return LPAREN;
")"						              return LISTR;
^lambda$				            return LAMBDA; 
[a-zA-z][a-zA-Z0-9]*        yylval.name = yytext; return VAR;
[ \n\t\r]+                  return SPACE;
.+               						printf("error: %s is\n", yytext); yyerror("Invalid token, read in lexer");

%%

