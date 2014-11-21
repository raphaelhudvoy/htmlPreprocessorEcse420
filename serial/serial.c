#include <stdio.h>   /* required for file operations */

FILE *input;            /* declare the file pointer */



int main() {

	FILE *ptr_file;
    char buf[1000];

	ptr_file =fopen("input.hnrl", "r");

	if (!ptr_file) 
		return 1;

	while (fscanf(ptr_file, "%s", buf) !=EOF) {
		printf("allo\n");
       	printf("%s \n", buf);
	}


    fclose(ptr_file);


	return  0;

}