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

void printNode (node *current_node, int level) {

	int i;

	// printf("parent address is %p\n", current_node->parent);
	// printf("node address is %p\n", current_node);

	// Indentation
	for (i = 0; i < level; i++) {
		printf("\t");
	}

	//Node
	printf("<%s id=\"%s\" class=\"%s\">\n", current_node->element, current_node->id, current_node->class);
	// printf("child address is %p\n", current_node->first_child );
	// printf("sibling address is %p\n", current_node->right_sibling );

}

void printTree (node *root) {

	printf("root address is %p\n", root);
	printf("root parent address %p\n", root->parent);

	int level = 0, i = 0;
	node *current_node = root, temp;
	
	printf("<%s id=\"%s\" class=\"%s\">\n", current_node->element, current_node->id, current_node->class);

	while (i < 50) {


		if (current_node->first_child != NULL) {
			current_node = current_node->first_child;
			printNode(current_node, ++level);
		}

		else if (current_node->right_sibling != NULL) {
			current_node = current_node->right_sibling;
			printNode(current_node, level);
		}

		else if (current_node->parent == NULL) {
			break;
		}

		else {
			current_node = current_node->parent;
			current_node->first_child = NULL;
			level--;
			printNode(current_node, level);
		}

		// printf("%p\n", current_node->parent);


		i++;
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
				current_node->first_child = malloc(sizeof(node));
				current_node->first_child->parent = current_node;
				
				current_node = current_node->first_child;

			break;

			case '}':


				if (current_node->parent != NULL)
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

				parseNode(new_node, buf);
				memcpy(current_node, new_node, sizeof(node));

				// if (parent_node != NULL) {
				// 	current_node->parent = parent_node;
				// }

				printf("%s, id is %s and class is %s with parent address of %p\n", current_node->element, current_node->id, current_node->class, current_node->parent);
		}
	}


    fclose(ptr_file);

    printTree(root);


	return  0;

};