
%code requires {
#include <stdio.h>
#include <stdlib.h>

#include "ast.h"
#include "interpreter.h"
  
  void yyerror(const char *s);
	
  // declaration of lexical analyzer
  extern int yylex();
  extern int yyparse();

  node *topLevel;
 }

%union {
  int digit;
  char *name;
  int bool;
  char character;
  node *astNode;
}

%token <digit> NUM BOOL
%token <character> CHAR
%token <name> STR DEF VAR
%token LAMBDA ADD SUB MUL DIV IF LISTL LISTR LPAREN SPACE

%type <astNode> top macro lambda list if name variable bool application

%start startProduction

%%

startProduction : top { printf("start value: %d, %s", $1->value, $1->name); topLevel = $1; }

top   : name							     { printf("Top name: %s\n", $1->name); $$ = insertNode(-1, STRNODE, $1->name, 0, NULL); }
| bool		 					           { $$ = insertNode(($1)->value, BOOLNODE, NULL, 0, NULL); }	 
| list                                                         { $$ = $1; }
| ADD top top 						 {  node **children = (node**)malloc(sizeof(node*) * 2); children[0] = $2; children[1] = $3; $$ = insertNode(-1, ADDNODE, NULL, 2, children); }
| SUB  top  top 						 {  node **children = (node**)malloc(sizeof(node*) * 2); children[0] = $2; children[1] = $3; $$ = insertNode(-1, SUBNODE, NULL, 2, children); }
| MUL  top  top 					 	 {  node **children = (node**)malloc(sizeof(node*) * 2); children[0] = $2; children[1] = $3; $$ = insertNode(-1, MULNODE, NULL, 2, children); }
| DIV  top  top  						 {  node **children = (node**)malloc(sizeof(node*) * 2); children[0] = $2; children[1] = $3; $$ = insertNode(-1, DIVNODE, NULL, 2, children); }
| LPAREN top LISTR  						 { $$ = $2; }
| lambda                                                       { $$ = $1; }
| macro                                                     	 { $$ = $1; }
| if	                                                         { $$ = $1; }    
| NUM                                                          { $$ = insertNode($1, NUMNODE, NULL, 0, NULL); }
| variable                                                     { $$ = $1; }
| application                                                  { $$ = $1; }
;	  

// for a macro with a lambda as the body, the name field is the name of the function, and the
// children[0] node is the parameter, with the children[1] node as the body
macro : DEF LPAREN variable LISTR LPAREN top LISTR                          {  node **children = (node**)malloc(sizeof(node*)); children[0] = $6; char *variableName = $3->name; $$ = insertNode(-1, DEFNODE, variableName, 1, children); }
// change the name member to be a child node of the node
| DEF  LPAREN variable variable LISTR  LPAREN top LISTR                        {  char *functionName = $3->name/*Function name */; node **children = (node**)malloc(sizeof(node*) * 2); children[0] = $4/*parameter*/; children[1] = $7; $$ = insertNode(-1, DEFNODE, functionName, 2, children); }
;

application: LPAREN top top LISTR { node **children = (node**)malloc(sizeof(node*) * 2); children[0] = $2; children[1] = $3; $$ = insertNode(-1, APPNODE, NULL, 2, children); }
;

// for a lambda, the parameter is the name
lambda: LAMBDA LPAREN variable LISTR  top LISTR                               {  node **children = (node**)malloc(sizeof(node*)); children[0] = $5; char *name = $3->name; $$ = insertNode(-1, LAMBDANODE, name, 1, children); }
;

name  : '"' STR '"'                                                           { char *insertName = $2; printf("Hit name production%s\n",insertName); $$ = insertNode(-1, STRNODE, insertName, 0, NULL); }
;
	  
bool  : BOOL                                                { $$ = insertNode(yylval.bool, BOOLNODE, NULL, 0, NULL); }
;
    
list  : LISTL LISTR                                                  { $$ = insertNode(-1, LISTNODE, NULL, 0, NULL); }
| LISTL top  list LISTR                                      { node **children = (node**)malloc(sizeof(node*)); children[0] = $2; children[1] = $3; $$ = insertNode(-1, LISTNODE, NULL, 2, children); }
;

if    : IF LISTL top LISTR  LPAREN top LISTR  LPAREN top LISTR              {  node **children = (node**)malloc(sizeof(node*) * 3); children[0] = $3; children[1] = $6; children[2] = $9; $$ = insertNode(-1, IFNODE, NULL, 3, children); }
;

variable : STR { char *name = $1; $$ = insertNode(-1, VARNODE, name, 0, NULL); }
;

%%

void yyerror(const char *errorMessage) {
  fprintf(stderr, "%s\n", errorMessage);
  exit(1);
}

int main() {
  yyparse();
	
  printf("%s %d\n", topLevel->name, topLevel->value);
  fflush(NULL);
  
  value *returnValue = process(topLevel, NULL);

  switch(returnValue->valueType) {
  case stringVal:
    printf("%s\n", (char *)(returnValue->pVal));
    break;
  default:
    printf("%d\n", *(int *)(returnValue->pVal));
    break;
  }

  return 0;
}
