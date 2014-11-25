#include <stdio.h>  
#include <stdlib.h>
#include <string.h>



int main() {


	

	int *a, *d;
	int b, c;
	a = &b;
	d = &c;

	b = 3;
	c = 4;

	printf("%d\n", b);
	printf("%d\n", *a);

	printf("%d\n", c);
	printf("%d\n", *d);

	d = a;
	a = &c;

	printf("%d\n", c);
	printf("%d\n", *a);

	printf("%d\n", b);
	printf("%d\n", *d);


	



	return  0;

};