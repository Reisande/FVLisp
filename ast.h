// enum containing all types/ operators/ operands
// associated with a set of number of children

// are enums constant?
enum tokens { NUM, STR, BOOL, ADD, SUB, MUL, DIV, DEF, MACRO, IF, LISTL, LISTR, LAMBDA } token;

typedef struct node {
	// number of pointers over to the children which act as operands/ nested operators
	enum tokens token; // type of the node
	struct node **children; // array of children
	int value; // associated store which holds enum value, corresponding to operands
	char *name; // in most cases the name string shouldn't be used, only times when this is needed is DEF, STR, LAMBDA(args)
};
 
// value corresponds to numeric value in cases of Num
// parent should probably be a sentinel node
// name is only used in DEF, STR, LAMBDA(args), otherwise is a null pointer
node *insertNode(int value, enum tokens tokenType, char *name, int numChildren, node **children) {
	node *insert = malloc(sizeof(node));
	insert->token = tokenType;
	insert->children = malloc(numChildren * sizeof(node));
	insert->name = name;
	inssert->value = value;
	
	int i;
	for(i = 0; i < numChildren; i++) {
		insert->children[i] = children[i];
	}
	
	return insert;
}

// further on is just stubs for the traditional binary tree

void delete(int value, enum token tokenType, char *name) {
	// should all values match? names should be unique, right? how would checking that work(preferably better than O(n))?
	// the only nodes which should ever be deleted are leaves
}

node *find(int value, enum token tokenType, char *name)  {
	return NULL;
}
