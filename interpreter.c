#include "ast.h"
#include <string.h>
#include <stdio.h>

// Change values to custom type
typedef struct value {
	// TODO: does defVal make logical sense?
  enum valueEnum { intVal, stringVal, lambdaVal, listVal, boolVal, defVal } valueType;
  void *pVal; // the actual value of the val(int, bool, etc.)
} value;

typedef struct listNode {
  value *Val; // can represent aything, just needs a typecast based off of token type
  struct listNode *next;
  enum nodeTypes nodeType;
  char * name;
} listNode;

typedef struct lambdaNode {
	// the body of the arguments
	node *body; 
	// the single argument. Multiple arguments could be implemented as multiple nested lambdas
	char *argument;
} lambdaNode;											

// I am also not going to implement lexical scoping, at least not yet. Evaluation
// will just be passing around this single stateNode around, adding to it as values
// are found. Since Scheme is garbage collected, I won't allow users to have direct
// access to memory addresses, so a stateNode will only have access to the variables
// the user declares
typedef struct stateNode {
  char *name;
  value *Val;
  struct stateNode *next;
} stateNode;

// finds the value a name is mapped to 
value *findValueInState(char *argName, stateNode *state) {
	if(state != NULL && argName != NULL) {
		if(strcmp(argName, state->name) == 0) {
			return state->Val;
		}
		else {
			return findValueInState(argName, state->next);
		}
	}
	else {
		return NULL;
	}
}

// checks if a value exists within the state already
int checkExistingNames(char *argName, stateNode *state) {
	return (findValueInState(argName, state) == NULL) ? 0 : 1;
}

void expandState(char *argName, value *insertValue, stateNode *currentState) {
	stateNode *newNode = (stateNode *) malloc(sizeof(stateNode));
	newNode->Val = insertValue;
	newNode->next = NULL;
	if(currentState != NULL) {
		stateNode *currentNode = currentState;
		while(currentNode->next != NULL) {
			currentNode = currentNode->next;
		}
		currentNode->next = newNode;
	}
	else {
		currentState = newNode;
	}
}

stateNode *removeValueFromState(char *argName, stateNode *currentState) {
	if(currentState == NULL) {
		return NULL;
	}
	else {
		if(strcmp(currentState->name, argName) == 0) {
			stateNode *temp = currentState->next;
			free(currentState);

			return temp;
		}
		else {
			stateNode *temp = currentState;
			while(currentState->next != NULL &&
						strcmp(currentState->next->name, argName) != 0) {
				temp = temp->next;
			}
			stateNode *freeNode = temp->next;
			temp->next = freeNode->next;
			free(freeNode);

			return currentState;
		}
	}
}

value *process(node *root, stateNode *state);
value *processIf(node *root, stateNode *state);

value *processDef(node *root, stateNode *state) {
	// TODO: check that the new value is not reassigning already assigned value.
	// If it is, it will be a type error, because shadowing existing values seems
	// unnecessarily complicated
	if(checkExistingNames(root->name, state) != 0) {
		value *returnDefVal;
		returnDefVal->valueType = defVal;
		returnDefVal->pVal = process(root->children[0], state);
		expandState(root->children[0]->name, (value *)(returnDefVal->pVal), state);
		return returnDefVal;
	}
	else {		
		return NULL;
	}
}

value *processIf(node *root, stateNode *state) {
  value *checkVal = process(root->children[0], state);
  if(checkVal->valueType == boolVal && (*((int*)(checkVal->pVal))) == 1) {
    return process(root->children[0], state);
  }
  else {
    return process(root->children[1], state);
  }
}

listNode *processList(node *root, stateNode *state) {
  if(root != NULL) {
    listNode *newNode = malloc(sizeof(node));
    newNode->Val = process(root->children[0], state);
    newNode->next = processList(root->children[1], state);
		return newNode;
  }
  else {
    return NULL;
  }
}

value *_processList(node *root, stateNode *state) {
	value *headNode = (value *) malloc(sizeof(value));
  headNode->valueType = listVal;
  headNode->pVal = (void *)processList(root, state);
	return headNode;
}

value *processVar(node *root, stateNode *state) {
	value *originalVal = findValueInState(root->name, state);
	if(originalVal != NULL) {
		value *returnVal = (value *) malloc(sizeof(value));
		memcpy(returnVal, originalVal, sizeof(value));

		return returnVal;
	}
	else {
		return NULL;
	}
}

// Process lambda is kind of a misnomer; This really just applies an argument
// to a lambda
value *processLambdaApplication(node *root, value *argument, stateNode *state) {
	// this function is simple. it is supposed to represent application of
	// a lambda to another value. Simply recurse through the AST and replace
	// any nodes with an identical name with a node with the value of the argument

	// this assumes that the parameter has been replaced, if it needs to be replaced
	if(root != NULL && argument != NULL) {
		// TODO: this just pushes the function name onto the stack, not the argument
		// should I assign an actual, random name to the function name for an anonymous
		// function?
		// actually, this should only happen for a lambda/function application, not an
		// anonymous function macro. So I need to make a separate processLambda and
		// processApplication
		expandState(root->children[0], argument, state);
		process(root, state);
		removeValueFromState(argument, state);
	}
	else {
		return NULL;
	}
}

// I should probably change this process into two separate process functions:
// one that parses through the AST and replaces all variables with their values,
// regardless of correctness, and another which evaluates after processing
value *process(node *root, stateNode *state) {
	if(state == NULL) {
		// builds a state by creating a head node
		state = (stateNode *) malloc(sizeof(stateNode));
	}

	if(root != NULL) {
		value *argument;
		value *returnVal = (value *) malloc(sizeof(value));
		int intermediaryInt = 0; // default value TODO: make a wrapper option struct
			
		switch (root->nodeType) {
    case NUMNODE:
      returnVal->valueType = intVal;
      returnVal->pVal = &(root->value);
      break;
    case STRNODE:
      returnVal->valueType = stringVal;
      returnVal->pVal = root->name;
      break;
    case BOOLNODE:
      returnVal->valueType = boolVal;
      returnVal->pVal = &(root->value);
      break;
    case ADDNODE:
      returnVal->valueType = intVal;
      intermediaryInt = (*((int *)(process(root->children[0], state)->pVal)) +
												 (*((int *)(process(root->children[0], state)->pVal))));
      returnVal->pVal = &intermediaryInt;
      break;
    case SUBNODE:
      returnVal->valueType = intVal;
      intermediaryInt = (*((int *)(process(root->children[0], state)->pVal)) -
												 (*((int *)(process(root->children[0], state)->pVal))));
      returnVal->pVal = &intermediaryInt;
      break;
    case MULNODE:
      returnVal->valueType = intVal;
      intermediaryInt = (*((int *)(process(root->children[0], state)->pVal)) *
												 (*((int *)(process(root->children[0], state)->pVal))));
      returnVal->pVal = &intermediaryInt;
      break;
    case DIVNODE:
      returnVal->valueType = intVal;
      intermediaryInt = (*((int *)(process(root->children[0], state)->pVal)) /
												 (*((int *)(process(root->children[0], state)->pVal))));
      returnVal->pVal = &intermediaryInt;
      break;
    case DEFNODE:
      returnVal = processDef(root, state);
			break;
    case IFNODE:
      returnVal = processIf(root, state);
		  break;
    case LISTNODE:
			returnVal->valueType = listVal;
      returnVal->pVal = _processList(root, state);
			break;
    case LAMBDANODE:
			returnVal->valueType = lambdaVal;
			returnVal->pVal = root;
			break;
		case APPNODE:
			argument = process(root->children[1], state);
			processLambdaApplication(root, argument, state);
			break;
		case VARNODE:
			returnVal = processVar(root, state);
			break;
    default:
      // should do nothing, this a stuck value
      returnVal = NULL; // should be an error value, but is not a very good one;
		}
		return returnVal;
	}
	else {
		printf("Some compilation error occurred\n");
		return NULL;
	}
}
