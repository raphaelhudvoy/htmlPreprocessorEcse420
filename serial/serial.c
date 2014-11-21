#include <stdio.h>  
#include <stdlib.h>
#include <string.h>

FILE *input; 
const int STRING_LENGTH = 100;         

typedef struct node {

	char *class;
	char *element;
	char *id;
	struct node *first_child;
	struct node *next_sibling;

		
} node;

node *root = NULL;

void parseNode (struct node *node, char *string) {

	// printf("-> %s\n", string);
	// strcpy(node->id, string);

	switch (string[0]) {
			case '#' :
				node->id = ++string;
				node->class = NULL;
				node->element = "div\0";

			break;

			case '.' :
				node->id = NULL;
				node->class = ++string;
				node->element = "div\0";
			break;

			case '{':

			break;

			case '}':
			break;

			default :
				node->id = NULL;
				node->class = NULL;
				node->element = "div\0";
		}

}


int main() {

	FILE *ptr_file;
    char buf[STRING_LENGTH];
	ptr_file =fopen("input.hnrl", "r");

	if (!ptr_file) 
		return 1;



	while (fscanf(ptr_file, "%s", buf) !=EOF) {

		//printf("%s\n", buf );


		struct node new_node;
		parseNode(&new_node, buf);

		printf("%s, id is %s and class is %s\n", new_node.element, new_node.id, new_node.class);


	}


    fclose(ptr_file);


	return  0;

};