#include "ast.h"

typedef struct value {
  enum valueEnum { intVal, string, closureVal, listVal, boolVal } valueType;
  void *pVal; // the actual value of the val(int, bool, etc.)
} value;

typedef struct listNode {
  value *Val; // can represent aything, just needs a typecast based off of token type
  struct listNode *next;
  enum tokens token;
  char * name;
} listNode;

typedef struct closure closure;
typedef struct stateNode stateNode;

struct closure {
  node *lambda;
  stateNode *stateVal;
};

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
  if(checkVal->valueType == boolVal && (*((int*)(checkVal->pVal))) == 1) {
    return process(root->children[0]);
  }
  else {
    return process(root->children[1]);
  }
}

value *processList(node *root) {
  if(root != NULL) {
    listNode *newNode = malloc(sizeof(node));
    newNode->Val = process(root->children[0]);
    newNode->next = processList(root->children[1]);
  }
  else {
    return NULL;
  }
}

value *process(node *root) {
  value *returnVal;
  int intermediaryInt = -99999;
  switch (root->token) {
    case NUM:
      returnVal->valueType = intVal;
      returnVal->pVal = &(root->value);
      break;
    case STR:
      returnVal->valueType = string;
      returnVal->pVal = root->name;
      break;
    case BOOL:
      returnVal->valueType = boolVal;
      returnVal->pVal = &(root->value);
      break;
    case ADD:
      returnVal->valueType = intVal;
      intermediaryInt = (*((int *)(process(root->children[0])->pVal)) + (*((int *)(process(root->children[0])->pVal))));
      returnVal->pVal = &intermediaryInt;
      break;
    case SUB:
      returnVal->valueType = intVal;
      intermediaryInt = (*((int *)(process(root->children[0])->pVal)) - (*((int *)(process(root->children[0])->pVal))));
      returnVal->pVal = &intermediaryInt;
      break;
    case MUL:
      returnVal->valueType = intVal;
      intermediaryInt = (*((int *)(process(root->children[0])->pVal)) * (*((int *)(process(root->children[0])->pVal))));
      returnVal->pVal = &intermediaryInt;
      break;
    case DIV:
      returnVal->valueType = intVal;
      intermediaryInt = (*((int *)(process(root->children[0])->pVal)) / (*((int *)(process(root->children[0])->pVal))));
      returnVal->pVal = &intermediaryInt;
      break;
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
  return returnVal;
}
