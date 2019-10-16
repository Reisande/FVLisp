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

value *process(node *root, stateNode *state);
