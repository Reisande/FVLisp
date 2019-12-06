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

typedef struct stateNode {
  char *name;
  value *Val;
  struct stateNode *next;
} stateNode;

typedef struct environment {
  value *Val;
  stateNode *state;
} environment;

environment *process(node *root, stateNode *state);
