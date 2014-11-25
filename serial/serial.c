#include <stdio.h>  
#include <stdlib.h>
#include <string.h>

FILE *input; 

typedef struct node {

	char class[50];
	char element[50];
	char id[50];
	char content[50];
	struct node *parent;
	struct node *first_child;
	struct node *right_sibling;
		
} node;

void parseNode (struct node *node, char *string) {

	//Initialize pointer
	node->parent = NULL;
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

void printTree (node *root) {

	printf("root address is %p\n", root);
	printf("root child class is %p\n", root->first_child);

	int level = 0, i;
	node *current_node = root;

	while (1) {
		if (current_node != NULL) {

			// print indentation
			for (i = 0; i < level; i++) {
				printf("\t");
			}

			printf("<%s id=\"%s\" class=\"%s\">\n", current_node->element, current_node->id, current_node->class);

			current_node = current_node->first_child;
			level++;


		}

		else {
			break;
		}
	}
}


int main() {

	FILE *ptr_file;
    char buf[100];
	ptr_file =fopen("input.hnrl", "r");

	node *root = malloc(sizeof(node));
	node *current_node = root;
	node *parent_node = NULL;



	if (!ptr_file) 
		return 1;



	while (fscanf(ptr_file, "%s", buf) !=EOF) {

		node *new_node;

		switch (buf[0]) {
			case '{': 
				parent_node = current_node;
				current_node->first_child = malloc(sizeof(node));
				current_node = current_node->first_child;

			break;

			case '}':

				if (parent_node != NULL)
					current_node = parent_node;
				// printf("end of children\n");

				if (buf[1] == ',') 
				{
					current_node->right_sibling = malloc(sizeof(node));
					current_node = current_node->right_sibling;
					// printf("next is sibling\n");
				}

			break;

			// case '(':


			default : 


				new_node = malloc(sizeof(node));

				parseNode(new_node, buf);
				memcpy(current_node, new_node, sizeof(node));

				if (parent_node != NULL) {
					new_node->parent = parent_node;
				}

				printf("%s, id is %s and class is %s\n", new_node->element, new_node->id, new_node->class);
		}
	}


    fclose(ptr_file);

    printTree(root);


	return  0;

};