#include <stdlib.h>
#include "ast.h"

// value corresponds to numeric value in cases of Num
// parent should probably be a sentinel node
// name is only used in DEF, STR, LAMBDA(args), otherwise is a null pointer
node *insertNode(int value, enum nodeTypes nodeType, char *name, int numChildren, node **children) {
	node *insert = malloc(sizeof(node));
	insert->nodeType = nodeType;
	insert->children = malloc(numChildren * sizeof(node));
	insert->name = name;
	insert->value = value;
	
	int i;
	for(i = 0; i < numChildren; i++) {
		insert->children[i] = children[i];
	}
	
	return insert;
}
