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
// insertNode(int value, enum tokens tokenType, char *name, int numChildren, node *children)
top   : name							     { printf(name); }
	  | bool		 						 
	  | list
	  | '+' top top 						 {  node **children = (node**)malloc(sizeof(node*) * 2); children[0] = $1; children[1] = $2; $$ = insertNode(-1, ADD, NULL, 2, children); }
	  | '-' top top 						 
	  | '*' lev2  lev2					 	 
	  | '/' lev2 lev2 						 
	  | '(' top ')'  						 
	  | '-' lev2
	  | lev1
	  | lambda
	  | macro
	  | if	  
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
	  | '\'' '(' STR ' ' list ')'
	  | '\'' '(' NUM ' ' list ')'
	  | '\'' '(' CHAR ' ' list ')'
	  | '\'' '(' list ' ' list ')'
	  ; 

if    : '(' "if " '(' top ')' ' ' '(' top ')' ' ' '(' top ')'
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