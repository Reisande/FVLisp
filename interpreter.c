#include "ast.h"
#include "interpreter.h"
#include <string.h>
#include <stdio.h>

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

stateNode *expandState(char *argName, value *insertValue, stateNode *currentState) {
  stateNode *newNode = (stateNode *) malloc(sizeof(stateNode));
  newNode->Val = (value *)malloc(sizeof(value));
  newNode->Val = insertValue;
  newNode->name = (char *) malloc(strlen(argName) + 1);
  strcpy(newNode->name, argName);
  newNode->next = currentState;
  return newNode;
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
	  while(currentState != NULL &&
			currentState->next != NULL &&
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

value *processIf(node *root, stateNode *state);

// variable shadowing exists in this language, so we don't check for existance of the
// new node
environment *processDef(node *root, stateNode *state) {
  environment *returnEnvironment = (environment *)malloc(sizeof(environment));
  returnEnvironment = process(root->children[0], state);
  
  value *returnDefVal = (value *)malloc(sizeof(value));
  returnDefVal->valueType = defVal;
  returnDefVal->pVal = returnEnvironment->Val;

  returnEnvironment->state =
	expandState(root->name, (value *)(returnDefVal->pVal), returnEnvironment->state);
  returnEnvironment->Val = returnDefVal;

  return returnEnvironment;
}

value *processIf(node *root, stateNode *state) {
  value *checkVal = process(root->children[0], state)->Val;
  if(checkVal->valueType == boolVal && (*((int*)(checkVal->pVal))) == 1) {
	return process(root->children[0], state)->Val;
  }
  else {
	return process(root->children[1], state)->Val;
  }
}

// TODO: check logic for base case
// TODO: maybe it would be cool to make separate pthreads for processing val and next
listNode *processList(node *root, stateNode *state) {
  if(root != NULL) {
	listNode *newNode = malloc(sizeof(node));
	newNode->Val = process(root->children[0], state)->Val;
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

// TODO: rewrite process lambda to use two values rather than processing inside of scope 

// Process lambda is kind of a misnomer; This really just applies an argument
// to a lambda
value *processLambdaApplication(node *root, value *argument, stateNode *state) {
  // this function is simple. it is supposed to represent application of
  // a lambda to another value. Simply recurse through the AST and replace
  // any nodes with an identical name with a node with the value of the argument

  // this assumes that the parameter has been replaced, if it needs to be replaced
  if(root != NULL && argument != NULL) {
	// actually, this should only happen for a lambda/function application, not an
	// anonymous function macro. So I need to make a separate processLambda and
	// processApplication
	stateNode *newState = expandState(root->children[0]->children[0]->name, argument, state);
	value *returnValue = (value *)malloc(sizeof(value));
	value *resultOfProcess = (process(root->children[0]->children[1], newState)->Val);
	if(resultOfProcess != NULL) {
	  *returnValue = *resultOfProcess;
	}
	removeValueFromState(root->children[0]->children[0]->name, newState);
	return returnValue;
  }
  else {
	printf("Either root or argument was not found within the state\n");
	return NULL;
  }
}

environment *process(node *root, stateNode *state) {
  if(root != NULL) {
	value *argument = (value *)malloc(sizeof(value));
	value *returnVal = (value *)malloc(sizeof(value));

	int intermediaryInt = 0; // default value TODO: make a wrapper option struct

	environment *returnEnvironment = (environment *)malloc(sizeof(environment));
	returnEnvironment->Val = returnVal;
	returnEnvironment->state = state;
	
	switch (root->nodeType) {
	case SEQNODE:
	  // take the top value of the first sequence, and throw it away
	  returnEnvironment = process(root->children[0], state);
	  returnEnvironment = process(root->children[1], returnEnvironment->state);
	  memcpy(returnVal, returnEnvironment->Val, sizeof(value));
	  break;

    case NUMNODE:
      returnVal->valueType = intVal;
      returnVal->pVal = &(root->value);
      break;

	case STRNODE:
      returnVal->valueType = stringVal;
      returnVal->pVal = (char *)malloc(strlen(root->name));
      strcpy(returnVal->pVal, root->name);
      break;
	  
    case BOOLNODE:
      returnVal->valueType = boolVal;
      returnVal->pVal = &(root->value);
      break;
	  
    case ADDNODE:
      returnVal->valueType = intVal;
	  returnVal->pVal = malloc(sizeof(int));
      *(int *)returnVal->pVal = (*((int *)(process(root->children[0], state)->Val->pVal)) +
			 (*((int *)(process(root->children[1], state)->Val->pVal))));;
      break;

    case SUBNODE:
      returnVal->valueType = intVal;
	  returnVal->pVal = malloc(sizeof(int));
      *(int *)returnVal->pVal = (*((int *)(process(root->children[0], state)->Val->pVal)) -
			 (*((int *)(process(root->children[1], state)->Val->pVal))));
      break;

	case MULNODE:
      returnVal->valueType = intVal;
      returnVal->pVal = malloc(sizeof(int));
      *(int *)returnVal->pVal = (*((int *)(process(root->children[0], state)->Val->pVal)) *
			 (*((int *)(process(root->children[1], state)->Val->pVal))));
      break;

	case DIVNODE:
      returnVal->valueType = intVal;
      returnVal->pVal = malloc(sizeof(int));
      *(int *)returnVal->pVal = (*((int *)(process(root->children[0], state)->Val->pVal)) /
			 (*((int *)(process(root->children[1], state)->Val->pVal))));
      break;

	case DEFNODE:
      returnEnvironment = processDef(root, state);
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
      returnEnvironment = process(root->children[1], state);
      returnVal = processLambdaApplication(root, returnEnvironment->Val, returnEnvironment->state);
      break;
	  
    case VARNODE:
      returnVal = processVar(root, state);
      break;

    default:
      // should do nothing, this a stuck value
      returnVal = NULL; // should be an error value, but is not a very good one;
    }
	returnEnvironment->Val = returnVal;
	
    return returnEnvironment;
  }
  else {
    printf("Some interpretation error occurred\n");
    return NULL;
  }
}
