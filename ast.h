// enum containing all types/ operators/ operands
// associated with a set of number of children
#include <stdlib.h>

enum nodeTypes { NUMNODE, STRNODE, BOOLNODE, ADDNODE, SUBNODE, MULNODE, DIVNODE, DEFNODE, IFNODE, LISTNODE, LAMBDANODE, VARNODE, APPNODE, SEQNODE } nodeType;

typedef struct node {
    // number of pointers over to the children which act as operands/ nested operators
  enum nodeTypes nodeType; // type of the node

  int numChildren; // used for recursively replacing variables with values
  struct node **children;
  // associated store which holds enum value, corresponding to operands
  // Maybe I should make this an int * so that in variable name cases, the value could just
  // be a NULL, and when the name is supposed to be a string, then it should be an actual value
  int value;

  // in most cases the name string shouldn't be used, only times when this is needed is DEF, STR, LAMBDA(args)
  char *name;
} node;
 
node *insertNode(int value, enum nodeTypes nodeType, char *name, int numChildren, node **children);
