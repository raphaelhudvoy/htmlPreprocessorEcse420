#include <stdio.h>   /* required for file operations */

FILE *input;            /* declare the file pointer */


int main() {

	FILE *ptr_file;
    char buf[1000];

	ptr_file =fopen("input.hnrl", "r");

	if (!ptr_file) 
		return 1;

	int newComponent = 1;
	char output[1000];
	int count = 0;

	while (fscanf(ptr_file, "%s", buf) !=EOF) {

		switch (buf[0]) {
			case '#' :
				

			break;

			case '.' :
			break;

			case '{':
			break;

			case '}':
			break;

			default :
				// if (buf != ' ') {

				// 	if (newComponent == 1) {
				// 		output[count] = '<';
				// 		count++;
				// 		output[count] = buf;

				// 	}

				// }
		}


       printf("%s\n", buf);
       printf("%c\n", buf[0]);
	}


    fclose(ptr_file);


	return  0;

}