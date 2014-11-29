#include <stdio.h>  
#include <stdlib.h>
#include <string.h>
#include <mpi.h>


typedef struct node {

	char class[50];
	char element[50];
	char id[50];
	struct node *parent;
	struct node *first_child;
	struct node *right_sibling;

	int initialized;

	//parallel variable
	int count;
	int seen;
		
} node;

void parseNode (struct node *node, struct node *parent, char *string) {

	//Initialize pointer
	node->parent = parent;
	node->first_child = NULL;
	node->right_sibling = NULL;
	node->initialized = 1;

	switch (string[0]) {
			case '#' :
				strcpy(node->id, ++string);
				strcpy(node->element, "div");

			break;

			case '.' :
				strcpy(node->class, ++string);
				strcpy(node->element, "div");
			break;

			default :
				strcpy(node->element, string);
		}
}

void printBeginningNode (node *current_node, int level) {

	int i;

	// Indentation
	for (i = 0; i < level; i++) {
		printf("\t");
	}

	//Node
	printf("<%s id=\"%s\" class=\"%s\">\n", current_node->element, current_node->id, current_node->class);

}

void printEndNode (node *current_node, int level) {

	int i;

	// Indentation
	for (i = 0; i < level; i++) {
		printf("\t");
	}

	printf("</%s>\n", current_node->element);
}

void printTree (node *root) {

	printf("root address is %p\n", root);
	printf("root parent address %p\n", root->parent);

	int level = 0, i = 0;
	node *current_node = root, temp;
	
	printBeginningNode(current_node, level);

	do {

		//removing unitialize node
		// if (current_node->first_child->initialized == 0) {
		// 	free(current_node->first_child);
		// } 

		// if (current_node->right_sibling->initialized == 0) {
		// 	current_node->right_sibling = NULL;
		// }

		if (current_node->first_child != NULL) {
			current_node = current_node->first_child;
			printBeginningNode(current_node, ++level);
		}

		else if (current_node->right_sibling != NULL) {
			current_node = current_node->right_sibling;
			printBeginningNode(current_node, level);
		}
		else {
			current_node = current_node->parent;
			current_node->first_child = NULL;
			level--;
			printEndNode(current_node, level);
		}

	} while (current_node->parent != NULL);
}

void buildTree (struct node *root) {

	FILE *ptr_file;
    char buf[100];
	ptr_file =fopen("input-medium.hnrl", "r");

	node *current_node = root;

	while (fscanf(ptr_file, "%s", buf) !=EOF) {

		node *new_node;

		switch (buf[0]) {
			case '{': 
				current_node->first_child = malloc(sizeof(node));
				current_node->first_child->parent = current_node;
				current_node->first_child->initialized = 0;

				current_node = current_node->first_child;

			break;

			case '}':

				current_node = current_node->parent;

				if (buf[1] == ',') 
				{
					current_node->right_sibling = malloc(sizeof(node));
					current_node->right_sibling->parent = current_node->parent;
					current_node = current_node->right_sibling;
				}

			break;

			default : 


				new_node = malloc(sizeof(node));

				parseNode(new_node, current_node->parent, buf);
				memcpy(current_node, new_node, sizeof(node));
		}
	}

	fclose(ptr_file);
}


void computeOutputInParallel (struct node *root, int rank, int size) {

	node *current_node = root;

	if (rank == 0) {
		printTree(root);


		while(1) {
			current_node->count = 1;

			// move down to the last child
			if (current_node->first_child != NULL && current_node->first_child->initialized == 1) {
				current_node = current_node->first_child;
			
			} else {
				current_node->count = 1;
				current_node->parent->count++;

				// move to right sibling
				// if (current_node->right_sibling != NULL && current_node->right_sibling->initialized == 1) {
				// 	current_node == current_node->right_sibling;
				// }
			}

		}
	}

}

int main(int argc, char **argv) {

	node *root = malloc(sizeof(node));
	buildTree(root);

	int parallel = 1;

	if (parallel == 1) {

		int rank, size;		
		MPI_Init(&argc, &argv);
		MPI_Comm_rank(MPI_COMM_WORLD, &rank);
		MPI_Comm_size(MPI_COMM_WORLD, &size);
		MPI_Status status;

		computeOutputInParallel(root, rank, size);


		MPI_Finalize();
	

	} else {
		// serial version here
	}

	return  0;

};