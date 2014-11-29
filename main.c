#include <stdio.h>  
#include <stdlib.h>
#include <string.h>
#include <omp.h>

//compile with -> /usr/local/bin/gcc -fopenmp main.c -o test
int printCount = 0;


typedef struct node {

	char class[50];
	char element[50];
	char id[50];
	struct node *parent;
	struct node *first_child;
	struct node *right_sibling;


	//parallel variable
	int count;
	int numberOfChild;

	//print variable
	int printed;
		
} node;

void parseNode (struct node *node, struct node *parent, char *string) {

	//Init flags
	node->count = 0;
	node->numberOfChild = 0;
	node->printed = 0;

	//Initialize pointer
	node->parent = parent;
	node->first_child = NULL;
	node->right_sibling = NULL;

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

	printCount++;

	printf("root address is %p\n", root);
	printf("root parent address %p\n", root->parent);

	int level = 0, i = 0;
	node *current_node = root, temp;
	node *parent_node = current_node->parent;

	current_node->parent = NULL;
	
	printBeginningNode(current_node, level);

	do {

		if (current_node->first_child != NULL && current_node->first_child->printed != printCount) {
			current_node = current_node->first_child;
			printBeginningNode(current_node, ++level);
		}

		else if (current_node->right_sibling != NULL) {
			current_node = current_node->right_sibling;
			printBeginningNode(current_node, level);
		}
		else {
			current_node = current_node->parent;
			current_node->first_child->printed = printCount;
			printEndNode(current_node, --level);
		}

	} while (current_node->parent != NULL);

	current_node->parent = parent_node;
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


void computeOutputInParallel (struct node *root) {

	node *current_node = root;
	int workByProcessor = 4;
	int threadID;

	// printTree(root);

	printf("starting with node\n");
	printf("<%s id=\"%s\" class=\"%s\">\n", current_node->element, current_node->id, current_node->class);
	printf("\n");

	while(1) {


		// move down to the last child
		if (current_node->first_child != NULL && current_node->first_child->count == 0) {
			current_node = current_node->first_child;
		
		} else { 

			if (current_node->count == 0) //means it is a leaf
				current_node->count = 1;

			current_node->parent->count += current_node->count;
			current_node->parent->numberOfChild++;

			// move to right sibling
			if (current_node->right_sibling != NULL ) {
				printf("<%s id=\"%s\" class=\"%s\">\n", current_node->element, current_node->id, current_node->class);
				current_node = current_node->right_sibling;
			}

			// move up to parent
			else {
				current_node = current_node->parent;

				// current node is now the parent
				current_node->count++; //adding 1 for parent node

				if (current_node->count >= workByProcessor || current_node->parent == NULL) {
					//need to parallelize the tree
					printf("<%s id=\"%s\" class=\"%s\">\n", current_node->element, current_node->id, current_node->class);
					printf("count is %d\n", current_node->count );
					printf("number of child is %d\n", current_node->numberOfChild );
					printf("parent is %p\n", current_node->parent );
					printf("right_sibling is %p\n", current_node->right_sibling );

					#pragma omp parallel num_threads(current_node->numberOfChild)
					 {
					   // Get thread number
					   threadID = omp_get_thread_num();
					   printf("\tSection 1 - I'm thread %d\n", threadID);
					 }

					current_node->count = 1;
					printTree(current_node);

					if (current_node->parent == NULL)
						break;
				}

			}

		}

	}
	

}

int main(int argc, char **argv) {

	node *root = malloc(sizeof(node));
	buildTree(root);

	printf("########### final tree ###########\n");
	printTree(root);
	printf("==================================\n\n");

	int parallel = 1;

	if (parallel == 1) {


		computeOutputInParallel(root);
	

	} else {
		// serial version here
	}

	return  0;

};