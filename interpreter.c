#include "ast.h"

typedef struct listNode {
  int value; // can represent aything, just needs a typecast based off of token type
  struct listNode *next;
  enum tokens token;
  char * name;
} listNode;

typedef struct closure closure ;
typedef struct stateNode stateNode;

struct closure {
  node *lambda;
  stateNode *stateVal;
};

typedef struct value {
  enum valueType { intVal, string, closureVal, listVal } value;
  void *pVal;
} value;

struct stateNode {
  char *name;
  value Val ;
  stateNode *next;
};

value process(node *root);
value processIf(node *root);

value *processLambda(node *root) {
  return NULL;
}

value *processDef(node *root) {
  return NULL;
}

value processIf(node *root) {
  value checkVal = process(root->children[0]);
  if( == 1) {
    return process(root->children[0]);
  }
  else {
    return process(root->children[1]);
  }
}

value *processList(node *root) {
  if(root != NULL) {
    listNode *newNode = malloc(sizeof(node));
    newNode->value = process(root->children[0]);
    newNode->next = processList(root->children[1]);
  }
  else {
    return NULL;
  }
}

// maybe I should change this return type to a specific struct which has a related token type, expecially
// in the case of handling lambdas, macros, Lists
value process(node *root) {
  switch (root->token) {
    case NUM:
      return root->value;
      break;
    case STR:
      printf("%s", root->name);
      return -1;
      break;
    case BOOL:
      return token->value;
      break;
    case ADD:
      return (process(root->children[0]) + process(root->children[1]);
      break;
    case SUB:
      return (process(root->children[0]) - process(root->children[1]);
      break;
    case MUL:
      return (process(root->children[0]) * process(root->children[1]);
      break;
    case DIV:
      return (process(root->children[0]) / process(root->children[1]);
      break;
    case DEF:
      return processDef(root);
      break;
    case IF:
      return processIf(root);
      break;
    case LIST:
      return processList(root);
      break;
    case LAMBDA:
      return processLambda(root);
      break;
    default:
      // should do nothing, this a stuck value
      return -1; // should be an error value, but is not a very good one;
  }
}
