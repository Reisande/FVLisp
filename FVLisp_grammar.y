
%code requires {
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
  
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
%token <name> STR VAR
%token LAMBDA ADD SUB MUL DIV IF LISTL LISTR LPAREN SPACE DEF NEWLINE

%type <astNode> top macro lambda list if name variable bool application top_seq

%start startProduction

%left NEWLINE

%%

startProduction : top_seq NEWLINE { topLevel = $1; }

top_seq : top_seq NEWLINE top_seq { printf("Hit sequence production\n"); node **children = (node**)malloc(sizeof(node*) * 2); children[0] = $1; children[1] = $3; $$ = insertNode(-1, SEQNODE, NULL, 2, children); }
| top  { $$ = $1; }

top   :  name { $$ = insertNode(-1, STRNODE, $1->name, 0, NULL); }
| bool { $$ = insertNode(($1)->value, BOOLNODE, NULL, 0, NULL); }	 
| LISTL list { $$ = $2; }
| ADD top top { node **children = (node**)malloc(sizeof(node*) * 2); children[0] = $2; children[1] = $3; $$ = insertNode(-1, ADDNODE, NULL, 2, children); }
| SUB top top { node **children = (node**)malloc(sizeof(node*) * 2); children[0] = $2; children[1] = $3; $$ = insertNode(-1, SUBNODE, NULL, 2, children); }
| MUL top top { node **children = (node**)malloc(sizeof(node*) * 2); children[0] = $2; children[1] = $3; $$ = insertNode(-1, MULNODE, NULL, 2, children); }
| DIV top top { node **children = (node**)malloc(sizeof(node*) * 2); children[0] = $2; children[1] = $3; $$ = insertNode(-1, DIVNODE, NULL, 2, children); }
| LPAREN top LISTR { $$ = $2; }
| lambda { $$ = $1; }
| macro { $$ = $1; }
| if { $$ = $1; }    
| NUM { $$ = insertNode($1, NUMNODE, NULL, 0, NULL); }
| variable { printf("Hit variable production\n"); $$ = $1; }
| application { $$ = $1; }
;	  

// for a macro with a lambda as the body, the name field is the name of the function, and the
// children[0] node is the parameter, with the children[1] node as the body
macro : LPAREN DEF LPAREN variable LISTR LPAREN top LISTR LISTR { node **children = (node**)malloc(sizeof(node*)); children[0] = $7; char *variableName = $4->name; printf("Macro definition: %s\n", variableName); $$ = insertNode(-1, DEFNODE, variableName, 1, children); }
| DEF variable top { node **children = (node**)malloc(sizeof(node*)); children[0] = $3; char *variableName = $2->name; printf("Macro definition: %s\n", variableName); $$ = insertNode(-1, DEFNODE, variableName, 1, children); }
;

application : LPAREN top top LISTR { node **children = (node**)malloc(sizeof(node*) * 2); children[0] = $2; children[1] = $3; $$ = insertNode(-1, APPNODE, NULL, 2, children); }
;

// for a lambda, the parameter is the name
lambda : LPAREN LAMBDA LPAREN variable LISTR LPAREN top LISTR LISTR {  node **children = (node**)malloc(sizeof(node*) * 2); children[0] = $4; children[1] = $7; char *name = $4->name; $$ = insertNode(-1, LAMBDANODE, name, 2, children); }
;
 
name : STR { char *insertName = $1; printf("Hit name production %s\n",insertName); $$ = insertNode(-1, STRNODE, insertName, 0, NULL); }
;
	  
bool : BOOL { $$ = insertNode(yylval.bool, BOOLNODE, NULL, 0, NULL); }
;
    
list  : LPAREN LISTR { $$ = insertNode(-1, LISTNODE, NULL, 0, NULL); }
| LPAREN top list LISTR { node **children = (node**)malloc(sizeof(node*)); children[0] = $2; children[1] = $3; $$ = insertNode(-1, LISTNODE, NULL, 2, children); }
;

if : IF LISTL top LISTR LPAREN top LISTR LPAREN top LISTR { node **children = (node**)malloc(sizeof(node*) * 3); children[0] = $3; children[1] = $6; children[2] = $9; $$ = insertNode(-1, IFNODE, NULL, 3, children); }
;

variable : VAR { $$ = insertNode(-1, VARNODE, $1, 0, NULL); }
;

%%

void yyerror(const char *errorMessage) {
  fprintf(stderr, "%s\n", errorMessage);
  exit(1);
}

int main() {

  while(yyparse());
  
  fflush(NULL);

  
  value *returnValue = process(topLevel, NULL)->Val;

  if(returnValue != NULL) {  
	switch(returnValue->valueType) {
	case stringVal:
	  printf("%s\n", (char *)(returnValue->pVal));
	  break;
	default:
	  printf("%d\n", *(int *)(returnValue->pVal));
	  break;
	}
  }

  return 0;
}
