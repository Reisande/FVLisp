%{
#include <stdio.h>
#include <stdlib.h>

// declaration of lexical analyzer
extern int yylex();
%}

%union {
	int digit;
	char *name;
	int bool;
	char character;
	Node *node;
	}

%token <digit> NUM
%token <character> CHAR
%token <name> STR
%token <name> DEF
%token LAMBDA

%type <node> top macro lambda list if name variable bool application

%%

top   : name							     { $$ = insertNode(-1, STR, $1, 0, NULL); }
| bool		 					 { $$ = insertNode($1, BOOL, NULL, 0, NULL); }	 
| list                                                         { $$ = $1; }
| '+' top top 						 {  node **children = (node**)malloc(sizeof(node*) * 2); children[0] = $2; children[1] = $3; $$ = insertNode(-1, ADD, NULL, 2, children); }
| '-' top top 						 {  node **children = (node**)malloc(sizeof(node*) * 2); children[0] = $2; children[1] = $3; $$ = insertNode(-1, SUB, NULL, 2, children); }
| '*' top top 					 	 {  node **children = (node**)malloc(sizeof(node*) * 2); children[0] = $2; children[1] = $3; $$ = insertNode(-1, MUL, NULL, 2, children); }
| '/' top top  						 {  node **children = (node**)malloc(sizeof(node*) * 2); children[0] = $2; children[1] = $3; $$ = insertNode(-1, DIV, NULL, 2, children); }
| '(' top ')'  						 { $$ = $2; }
//| '-' '0' ' ' top                                                      {  node **children = (node**)malloc(sizeof(node*) * 2);  children[0] = 0; children[1] = $2; $$ = insertNode(-1, SUB, NULL, 2, children); }
| lambda                                                       { $$ = $1; }
| macro                                                     	 { $$ = $1; }
| if	                                                         { $$ = $1; }    
| NUM                                                          { $$ = $1; }
| variable                                                     { $$ = $1; }
| application { $$ = $1; }
;	  

macro : DEF ' ' '(' variable ')' ' ' '(' top ')'                          {  node **children = (node**)malloc(sizeof(node*)); children[0] = $8; char *name = $4; $$ = insertNode(-1, DEF, name, 1, children); }
// change the name member to be a child node of the node
| DEF ' '  '(' variable variable ')' '(' top')'                        {  char *name = $4/*Function name */; node **children = (node**)malloc(sizeof(node*) * 2); children[0] = $5/*parameter*/; children[1] = $8; $$ = insertNode(-1, DEF, name, 2, children); }
;

application: '(' top ' ' top ')' { node **children = (node**)malloc(sizeof(node*) * 2); children[0] = $2; children[1] = $4; $$ = insertNode(-1, APP, NULL, 2, children); }
;

lambda: LAMBDA '(' variable ')' '(' top ')'                               {  node **children = (node**)malloc(sizeof(node*)); children[0] = $6; char *name = $3; $$ = insertNode(-1, LAMBDA, name, 1, children); }
;

name  : '"' STR '"'                                                           { char *insertName = $2; $$ = insertNode(-1, STR, insertName, 0, NULL); }
;
	  
bool  : '#' 't'                                                       { $$ = insertNode(1, BOOL, NULL, 0, NULL) }
| '#' 'f'                                                      { $$ = insertNode(0, BOOL, NULL, 0, NULL) }
;
	  
list  : '\'' '(' ')'                                                  { $$ = insert(-1, LIST, NULL, 0, NULL); }
| '\'' '(' top ' ' list ')'                                      { node **children = (node**)malloc(sizeof(node*)); children[0] = $3; children[1] = $5; $$ = insert(-1, LIST, NULL, 2, children); }
/*| '\'' '(' NUM ' ' list ')'                                      { node **children = (node**)malloc(sizeof(node*)); children[0] = $3; children[1] = $5; $$ = insert(-1, LIST, NULL, 2, children); }
| '\'' '(' CHAR ' ' list ')'                                     { node **children = (node**)malloc(sizeof(node*)); children[0] = $3; children[1] = $5; $$ = insert(-1, LIST, NULL, 2, children); }
| '\'' '(' list ' ' list ')'                                     { node **children = (node**)malloc(sizeof(node*)); children[0] = $3; children[1] = $5; $$ = insert(-1, LIST, NULL, 2, children); }
| '\'' '(' bool ' ' list ')'                                     { node **children = (node**)malloc(sizeof(node*)); children[0] = $3; children[1] = $5; $$ = insert(-1, LIST, NULL, 2, children); }
| '\'' '(' variable ' ' list ')'                                     { node **children = (node**)malloc(sizeof(node*)); children[0] = $3; children[1] = $5; $$ = insert(-1, LIST, NULL, 2, children); }
; */

if    : '(' "if " '(' top ')' ' ' '(' top ')' ' ' '(' top ')'              {  node **children = (node**)malloc(sizeof(node*) * 3); children[0] = $4; children[1] = $8; children[2] = $12; $$ = insertNode(-1, IF, NULL, 3, children); }
;

variable : STR { char *name = $1; $$ = insertNode(-1, VAR, name, 0, NULL); }
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
