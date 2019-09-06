#include "ast.h"

// Change values to custom type
typedef struct value {
	// TODO: does defVal make logical sense?
  enum valueEnum { intVal, stringVal, lambdaVal, listVal, boolVal, defVal } valueType;
  void *pVal; // the actual value of the val(int, bool, etc.)
} value;

typedef struct listNode {
  value *Val; // can represent aything, just needs a typecast based off of token type
  struct listNode *next;
  enum tokens token;
  char * name;
} listNode;

typedef struct lambdaNode {
	// the body of the arguments
	node *body; 
	// the single argument. Multiple arguments could be implemented as multiple nested lambdas
	char *argument;
} lambdaNode;											

typedef struct closure closure;
typedef struct stateNode stateNode;

// TODO: why did I have closures/state again?
struct closure {
  node *lambda;
  stateNode *stateVal;
};

struct stateNode {
  char *name;
  value Val;
  stateNode *next;
};

value *process(node *root);
value *processIf(node *root);

value *processDef(node *root) {
	// TODO: check that the new value is not reassigning already assigned value.
	// If it is, it will be a type error, because shadowing existing values seems
	// unnecessarily complicated
	if(process(root->children[0]) == stringVal) {
		value *returnDefVal;
		returnDefVal->valueType = defVal;
		returnDefVal->pVal = process(root->children[1]);
		return returnDefVal;
	}
	else {
		return NULL;
	}
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

listNode *processList(node *root) {
  if(root != NULL) {
    listNode *newNode = malloc(sizeof(node));
    newNode->Val = process(root->children[0]);
    newNode->next = processList(root->children[1]);
		return newNode;
  }
  else {
    return NULL;
  }
}

value *_processList(node *root) {
	value *headNode = (value *) malloc(sizeof(value));
  headNode->valueType = listVal;
  headNode->pVal = (void *)processList(root);
	return headNode;
}

value *process(node *root, stateNode *state) {
  value *returnVal = (value *) malloc(sizeof(value));
  int intermediaryInt = 0; // default value TODO: make a wrapper option struct
	if(root != NULL) {
		switch (root->token) {
    case NUM:
      returnVal->valueType = intVal;
      returnVal->pVal = &(root->value);
      break;
    case STR:
      returnVal->valueType = stringVal;
      returnVal->pVal = root->name;
      break;
    case BOOL:
      returnVal->valueType = boolVal;
      returnVal->pVal = &(root->value);
      break;
    case ADD:
      returnVal->valueType = intVal;
      intermediaryInt = (*((int *)(process(root->children[0])->pVal)) +
												 (*((int *)(process(root->children[0])->pVal))));
      returnVal->pVal = &intermediaryInt;
      break;
    case SUB:
      returnVal->valueType = intVal;
      intermediaryInt = (*((int *)(process(root->children[0])->pVal)) -
												 (*((int *)(process(root->children[0])->pVal))));
      returnVal->pVal = &intermediaryInt;
      break;
    case MUL:
      returnVal->valueType = intVal;
      intermediaryInt = (*((int *)(process(root->children[0])->pVal)) *
												 (*((int *)(process(root->children[0])->pVal))));
      returnVal->pVal = &intermediaryInt;
      break;
    case DIV:
      returnVal->valueType = intVal;
      intermediaryInt = (*((int *)(process(root->children[0])->pVal)) /
												 (*((int *)(process(root->children[0])->pVal))));
      returnVal->pVal = &intermediaryInt;
      break;
    case DEF:
      returnVal = processDef(root);
			break;
    case IF:
      returnVal = processIf(root);
		  break;
    case LIST:
			returnVal->valueType = listVal;
      returnVal->pVal = _processList(root);
			break;
    case LAMBDA:
			// how would I go about actually applying a lambda to a value?
			returnVal->valueType = lambdaVal;
			// TODO: this feels off to me, although I guess it does make sense for
			// lambdas to be terminals
			returnVal->pVal = root;
			break;
    default:
      // should do nothing, this a stuck value
      returnVal =  NULL; // should be an error value, but is not a very good one;
		}
		return returnVal;
	}
	else {
		printf("Some compilation error occurred\n");
		return NULL;
	}
}

int main() {

	return 0;
}
