X%{
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
          | bool		 					 { $$ = $1; }	 
          | list                                                         { $$ = $1; }
	  | '+' top top 						 {  node **children = (node**)malloc(sizeof(node*) * 2); children[0] = $2; children[1] = $3; $$ = insertNode(-1, ADD, NULL, 2, children); }
	  | '-' top top 						 {  node **children = (node**)malloc(sizeof(node*) * 2); children[0] = $2; children[1] = $3; $$ = insertNode(-1, SUB, NULL, 2, children); }
	  | '*' top top 					 	 {  node **children = (node**)malloc(sizeof(node*) * 2); children[0] = $2; children[1] = $3; $$ = insertNode(-1, MUL, NULL, 2, children); }
	  | '/' top top  						 {  node **children = (node**)malloc(sizeof(node*) * 2); children[0] = $2; children[1] = $3; $$ = insertNode(-1, DIV, NULL, 2, children); }
          | '(' top ')'  						 { $$ = $2; }
	  | '-' top                                                      {  node **children = (node**)malloc(sizeof(node*) * 2);  children[0] = 0; children[1] = $2; $$ = insertNode(-1, SUB, NULL, 2, children); }
	  | lambda                                                       { $$ = $1; }
	  | macro                                               	 { $$ = $1; }
	  | if	                                                         { $$ = $1; }    
	  | NUM                                                          { $$ = $1; }
	  ;	  
	  
macro : DEF ' ' '(' name ')' ' ' '(' top ')'                          {  node **children = (node**)malloc(sizeof(node*)); children[0] = $8; char *name = $4; $$ = insertNode(-1, DEF, name, 1, children); }
          | DEF ' '  '(' name name ')' '(' top')'                        {  char *name = $4/*Function name */; node **children = (node**)malloc(sizeof(node*) * 2); children[0] = $5/*parameter*/; children[1] = $8; $$ = insertNode(-1, DEF, name, 2, children); }
	  ;
	  
lambda: LAMBDA '(' name ')' '(' top ')'                               {  node **children = (node**)malloc(sizeof(node*)); children[0] = $6; char *name = $3; $$ = insertNode(-1, LAMBDA, name, 1, children); }

name  : STR                                                           { char *name = $1; $$ = insertNode(-1, STR, name, 0, NULL); }
          ;
	  
bool  : '#' 't'                                                       { $$ = insertNode(1, BOOL, NULL, 0, NULL) }
          | '#' 'f'                                                      { $$ = insertNode(0, BOOL, NULL, 0, NULL) }
	  ;
	  
list  : '\'' '(' ')'                                                  { $$ = insert(-1, LIST, NULL, 0, NULL); }
	  | '\'' '(' STR ' ' list ')'                                      { node **children = (node**)malloc(sizeof(node*)); children[0] = $3; children[1] = $5; $$ = insert(-1, LIST, NULL, 2, children); }
	  | '\'' '(' NUM ' ' list ')'                                      { node **children = (node**)malloc(sizeof(node*)); children[0] = $3; children[1] = $5; $$ = insert(-1, LIST, NULL, 2, children); }
	  | '\'' '(' CHAR ' ' list ')'                                     { node **children = (node**)malloc(sizeof(node*)); children[0] = $3; children[1] = $5; $$ = insert(-1, LIST, NULL, 2, children); }
	  | '\'' '(' list ' ' list ')'                                     { node **children = (node**)malloc(sizeof(node*)); children[0] = $3; children[1] = $5; $$ = insert(-1, LIST, NULL, 2, children); }
          | '\'' '(' bool ' ' list ')'                                     { node **children = (node**)malloc(sizeof(node*)); children[0] = $3; children[1] = $5; $$ = insert(-1, LIST, NULL, 2, children); }
	  ; 

if    : '(' "if " '(' top ')' ' ' '(' top ')' ' ' '(' top ')'              {  node **children = (node**)malloc(sizeof(node*) * 3); children[0] = $4; children[1] = $8; children[2] = $12; $$ = insertNode(-1, IF, NULL, 3, children); }
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
