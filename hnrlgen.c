#include <stdio.h>
#include <stdlib.h>
#include <time.h>
char *types[6] = {"div","table","form", "h1", "h2", "h3"};
int cur_id = 0;
FILE *output,*input;


void printByDepth(int depth, int branch,int cur_depth);

int main ( int argc, char *argv[] )
{
	
	 time_t t;
	 /* Intializes random number generator */
   	srand((unsigned) time(&t));


    if ( argc != 3 ) /* argc should be 3 for correct execution */
    {
        /* We print argv[0] assuming it is the program name */
        printf( "usage: %s -branching-factor -depth  \n", argv[0]);
        return -1;
    }
    
    output = fopen("output.html", "w");
    input = fopen("input.hnrl","w");

	if (output == NULL )
	{
	    printf("Error opening file!\n");
	    exit(1);
	}

	/* print some text */
	const char *text = "Write this to the file";
	
     
    printf("%d %d \n",atoi(argv[1]),atoi(argv[2]));

    int branchingFactor = atoi(argv[1]);
    int depth = atoi(argv[2]);
    printf("<body>\n");
    printByDepth(depth,branchingFactor,0);
    printf("</body>");
}
void printByDepth(int depth, int branch,int cur_depth) {
	int randomBranching = rand()%branch +1 ;
	
	if(depth==cur_depth)
		return;

	char *tabs = malloc(sizeof(char)*cur_depth);
	for(int k=0; k<cur_depth;k++)
		tabs[k]='\t';


	for(int i=0; i<randomBranching; i++) {
		
		char *elem = types[rand()%6];
		char *attr;
		char *attr_input;
		if(rand()%2 == 0)
		{
			 sprintf(attr, "class=testclass_%d", cur_depth);
			 sprintf(attr_input,".testclass_%d", cur_depth);
		}
		else {

			sprintf(attr, "id=testid_%d", cur_id);
			 sprintf(attr_input,"#testid_%d", cur_id);
			 cur_id++;
		}

		printf("%s<%s %s>\n",tabs,elem,attr);
		fprintf(output, "%s<%s %s>\n",tabs,elem,attr);
		fprintf(input, "%s %s {\n",tabs, attr_input);
		
    	printByDepth(depth,branch,cur_depth+1);
    	
    	printf("%s</%s>\n",tabs,elem);
    	fprintf(output, "%s</%s>\n",tabs,elem);
    	fprintf(input,"%s}\n",tabs);
    }
}