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
	char *content;

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
	node->content = NULL;

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

void computeBeginningNode (char* result, node *current_node, int level) {
	char element[100];
	int i = 0;
	node *child = current_node->first_child;
	result[0] = '\0';

	for (i = 0; i < level; i++) {
		strcat(result, "\t");
	}



	sprintf(element, "<%s id=\"%s\" class=\"%s\">\n", current_node->element, current_node->id, current_node->class);
	strcat(result, element);

	// Adding children content
	while (child != NULL) {
		printf("child address is %p\n", child);
		printf("done with node\n");
		printf("<%s id=\"%s\" class=\"%s\">\n", child->element, child->id, child->class);
		printf("sibling is %p\n", child->right_sibling);
		printf("child content is \n");
		printf("%s\n", child->content);

		strcat(result, child->content);

		printf("here\n");

		if (child->right_sibling == NULL)
			break;
		else 
			child = child->right_sibling;
	}

	printf("here\n");

	for (i = 0; i < level; i++) {
		strcat(result, "\t");
	}

	sprintf(element, "</%s>\n", current_node->element);
	strcat(result, element);


} 

void printEndNode (node *current_node, int level) {

	int i;

	// Indentation
	for (i = 0; i < level; i++) {
		printf("\t");
	}

	printf("</%s>\n", current_node->element);
}

void computeNodeToHtml (node *root, int level) {

	char parsedString[400000] = {};

	int i = 0;
	node *current_node = root;
	node *parent_node = current_node->parent;
	node *child;
	int length = 0;

	root->parent = NULL;

	printf("-- compute node --\n");
	printf("\t<%s id=\"%s\" class=\"%s\">\n", current_node->element, current_node->id, current_node->class);
	printf("first_child is %p\n", current_node->first_child);
	printf("right_sibling is %p \n", current_node->right_sibling);
	printf("level is %d\n", level);
	printf("first_child content is\n");
	printf("%s\n", current_node->first_child->content);
	printf("=========\n");
	do {

		if (current_node->first_child != NULL && current_node->first_child->content == NULL) {
			current_node = current_node->first_child;
			level++;
		}

		else {
			
			// make sure all children have content
			child = current_node->first_child;
			while (child != NULL) {
				if (child->content == NULL) {
					break;
				} 
				else 
					child = child->right_sibling;
			}

			if (child != NULL) 
				current_node = child;
			
			else { // all children have content
				
				if (current_node->right_sibling != NULL && current_node != root) {
			
					// compute current node before moving to its neighbor
					computeBeginningNode(parsedString, current_node, level);
					current_node->content = malloc(strlen(parsedString) + 1);
					strcpy(current_node->content, parsedString);

					// moving to the neighbor
					current_node = current_node->right_sibling;

				}
				else if (current_node->parent != NULL) {

					// compute current node before moving to its parent
					computeBeginningNode(parsedString, current_node, level);
					current_node->content = malloc(strlen(parsedString) + 1);
					strcpy(current_node->content, parsedString);

					current_node = current_node->parent;
					level--;
				}
			}
		}

	} while (current_node->parent != NULL);

	// compute the root
	computeBeginningNode(parsedString, current_node, level);
	current_node->content = malloc(strlen(parsedString) + 1);
	strcpy(current_node->content, parsedString);

	

	root->parent = parent_node;
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
	ptr_file =fopen("input.hnrl", "r");

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
	int workByProcessor = 8;
	int i, level = 0;
	char parsedString[100000];

	while(1) {


		// move down to the last child
		if (current_node->first_child != NULL && current_node->first_child->count == 0) {
			current_node = current_node->first_child;
			level++;
		
		} else { 

			if (current_node->count == 0) //means it is a leaf
				current_node->count = 1;

			current_node->parent->count += current_node->count;
			current_node->parent->numberOfChild++;

			// move to right sibling
			if (current_node->right_sibling != NULL ) {
				current_node = current_node->right_sibling;
			}

			// move up to parent
			else {
				current_node = current_node->parent;
				level--;

				// current node is now the parent
				current_node->count++; //adding 1 for parent node

				if (current_node->count >= workByProcessor || current_node->parent == NULL) {

					if (current_node->parent == NULL)
						printf("root reached\n");

					printf("Parallizing --------\n");
					printf("\t<%s id=\"%s\" class=\"%s\">\n", current_node->element, current_node->id, current_node->class);
					printf("number of child %d\n", current_node->numberOfChild);
					printf("----------------------\n");
					//need to parallelize the tree
					#pragma omp parallel num_threads(current_node->numberOfChild)
					{

						node *current_child = current_node->first_child;
						int threadID = omp_get_thread_num();

						// selecting the right child for each process
						for (i = 0; i < threadID; i++) {
							current_child = current_child->right_sibling; 
						}
						

						if (current_child != NULL && current_child->content == NULL) {
							// Get thread number
							// printf("\t<%s id=\"%s\" class=\"%s\">\n", current_child->element, current_child->id, current_child->class);
							computeNodeToHtml(current_child, level + 1);
							// printf("%s\n", current_child->content);
						}

					}
					
					// Compute parent node
					computeBeginningNode(parsedString, current_node, level);
					current_node->content = malloc(strlen(parsedString) + 1);
					strcpy(current_node->content, parsedString);

					current_node->count = 1;

					if (current_node->parent == NULL) {
						break;
					}
				}

			}

		}
	}


	

}

int main(int argc, char **argv) {


	FILE *output_p;
	node *root = malloc(sizeof(node));
	buildTree(root);

	int parallel = 1;

	if (parallel == 1) {


		computeOutputInParallel(root);

		// Print content to a file
		if (root->content != NULL) {
			output_p = fopen("output_parallel.html", "w");

			if (output_p != NULL) {
				fputs(root->content, output_p);
				fclose(output_p);
			} 
			else
				printf("Unable to open the output file\n");
		}

		else 
			printf("Error: no content\n");
	

	} else {
		// serial version here
	}

	return  0;

};