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
  enum valueEnum { intVal, string, closureVal, listVal, boolVal } valueType;
  void *pVal; // the actual value of the val(int, bool, etc.)
} value;

struct stateNode {
  char *name;
  value Val ;
  stateNode *next;
};

value *process(node *root);
value *processIf(node *root);

value *processLambda(node *root) {
  return NULL;
}

value *processDef(node *root) {
  return NULL;
}

value *processIf(node *root) {
  value *checkVal = process(root->children[0]);
  if(checkVal->valueType == boolVal && (int)(*(checkVal->pVal)) == 1) {
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

value *process(node *root) {
  switch (root->token) {
    value *returnVal;
    case NUM:
      returnVal->valueType = intVal;
      returnVal->pVal = &(root->value);
      return returnVal;
    case STR:
      returnVal->valueType = intVal;
      returnVal->pVal = root->name;
      return returnVal;
    case BOOL:
      returnVal->valueType = boolVal;
      returnVal->pVal = &(root->value);
      return returnVal;
    case ADD:
      return (*(process(root->children[0])->pVal) + (*(process(root->children[0])->pVal)));
    case SUB:
      return (*(process(root->children[0])->pVal) - (*(process(root->children[1])->pVal)));
    case MUL:
      return (*(process(root->children[0])->pVal) * (*(process(root->children[1])->pVal)));
    case DIV:
      return (*(process(root->children[0])->pVal) / (*(process(root->children[1])->pVal)));
    case DEF:
      return processDef(root);
    case IF:
      return processIf(root);
    case LIST:
      return processList(root);
    case LAMBDA:
      return processLambda(root);
    default:
      // should do nothing, this a stuck value
      return NULL; // should be an error value, but is not a very good one;
  }
}
