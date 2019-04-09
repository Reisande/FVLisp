%{
#include <stdio.h>
#include <stdlib.h>

// declaration of lexical analyzer
extern int yylex();
%}

%union {
	int d;
	char *name;
	int b;
	char c;
	}

%token <d> NUM
%token <c> CHAR
%token <name> STR
%token <name> DEF
%token LAMBDA

%%

top   : name							     { printf(name); }
	  | bool		 						 
	  | list
	  | lev1 '+' lev1 						 
	  | lev1 '-' lev1 						 
	  | lev1
	  | lambda
	  | macro
	  ;
	  
lev1  : lev2 '*' lev2					 	 
	  | lev2 '/' lev2 						 
	  ;
	  
lev2  : '(' top ')'  						 
	  | '-' lev2	 						 
	  | NUM
	  ;	  
	  
macro : DEF ' ' '(' name ')' ' ' '(' top ')' 
	  | DEF ' '  '(' name name ')' '(' top')'
	  ;
	  
lambda: LAMBDA '(' name ')' '(' top ')'

name  : STR
	  ;
	  
bool  : '#' 't' 
	  | '#' 'f' 
	  ;
	  
list  : '\'' '(' ')'
	  | '\'' '(' STR list ')'
	  | '\'' '(' NUM list ')'
	  | '\'' '(' CHAR list ')'
	  | '\'' '(' list list ')'
	  ; 

%%

void yyerror(char *errorMessage) {
	fprintf(stderr, "%s\n", errorMessage);
	exit(1);
}

int main() {
	yyparse();
	
	return 0;
}