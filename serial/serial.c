#include <stdio.h>  
#include <stdlib.h>
#include <string.h>

FILE *input; 

typedef struct node {

	char class[50];
	char element[50];
	char id[50];
	struct node *parent;
	struct node *first_child;
	struct node *right_sibling;
		
} node;

void parseNode (struct node *node, struct node *parent, char *string) {

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

void printBeginningNode (node *current_node, int level,FILE *file) {

	int i;

	// Indentation
	for (i = 0; i < level; i++) {
		printf("\t");
		fprintf(file, "\t");
	}

	//Node
	printf("<%s id=\"%s\" class=\"%s\">\n", current_node->element, current_node->id, current_node->class);
	fprintf(file,"<%s id=\"%s\" class=\"%s\">\n", current_node->element, current_node->id, current_node->class);
}

void printEndNode (node *current_node, int level,FILE *file) {

	int i;

	// Indentation
	for (i = 0; i < level; i++) {
		printf("\t");
		fprintf(file, "\t");
	}

	printf("</%s>\n", current_node->element);
	fprintf(file,"</%s>\n", current_node->element);
}

void printTree (node *root, FILE *file) {

	printf("root address is %p\n", root);
	printf("root parent address %p\n", root->parent);

	int level = 0, i = 0;
	node *current_node = root, temp;
	
	printBeginningNode(current_node, level,file);

	do {

		if (current_node->first_child != NULL) {
			current_node = current_node->first_child;
			printBeginningNode(current_node, ++level,file);
		}

		else if (current_node->right_sibling != NULL) {
			current_node = current_node->right_sibling;
			printBeginningNode(current_node, level,file);
		}
		else {
			current_node = current_node->parent;
			current_node->first_child = NULL;
			level--;
			printEndNode(current_node, level,file);
		}

	} while (current_node->parent != NULL);
}


int main() {

	FILE *ptr_file;
    char buf[100];
	ptr_file =fopen("input.hnrl", "r");

	node *root = malloc(sizeof(node));
	node *current_node = root;

	if (!ptr_file) 
		return 1;



	while (fscanf(ptr_file, "%s", buf) !=EOF) {

		node *new_node;

		switch (buf[0]) {
			case '{': 
				current_node->first_child = malloc(sizeof(node));
				current_node->first_child->parent = current_node;

				current_node = current_node->first_child;

			break;

			case '}':


				// if (current_node->parent != NULL)
					current_node = current_node->parent;
				// printf("end of children\n");

				if (buf[1] == ',') 
				{
					current_node->right_sibling = malloc(sizeof(node));
					current_node->right_sibling->parent = current_node->parent;
					current_node = current_node->right_sibling;
					// printf("next is sibling\n");
				}

			break;

			// case '(':


			default : 


				new_node = malloc(sizeof(node));

				parseNode(new_node, current_node->parent, buf);
				memcpy(current_node, new_node, sizeof(node));

				// if (parent_node != NULL) {
				// 	current_node->parent = parent_node;
				// }

				// printf("%s, id is %s and class is %s with node address of %p\n", current_node->element, current_node->id, current_node->class, current_node);
				// printf("parent address is %p\n", current_node->parent );
				// printf("child address is %p\n", current_node->first_child );
				// printf("sibling address is %p\n", current_node->right_sibling );
		}
	}


    fclose(ptr_file);

    
    FILE *out_file =fopen("output_serial.html", "w");
    printTree(root,out_file);

	return  0;

};