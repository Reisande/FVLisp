%{
#include "scanner_lexer.h"
%}

%%

[0-9]*					            yylval.d = atoi(yytext); return NUM;
\"[a-zA-z][a-zA-Z0-9]*\" 	  yylval.name = yytext; return STR;
(#t|#f) 				            yylval.b = strcmp((yytext), "#t") == 0 ? 1 : 0; return BOOL;
"+"						              return ADD;
"-"						              return SUB;
"*"						              return MUL;
"/"						              return DIV;
^def$					              return DEF;
^if$   					            return IF; 
"'("					              return LISTL; 
")"						              return LISTR;
^lambda$				            return LAMBDA; 
[a-zA-z][a-zA-Z0-9]*        yylval.name = yytext; return VAR;
.                						yyerror("Invalid token");

%%

