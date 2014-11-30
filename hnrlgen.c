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
    
    fprintf(input,"#root {\n");
	fprintf(output,"<div id=\"root\">\n");
    printByDepth(depth,branchingFactor,0);
     fprintf(input,"}\n");
     fprintf(output,"</div>\n");
     fclose(input);
     fclose(output);
  
}
void printByDepth(int depth, int branch,int cur_depth) {
	int randomBranching = rand()%branch +1 ;
	int k,i;

	char *tabs = malloc(sizeof(char)*(cur_depth+1));
	for(k=0; k<cur_depth;k++)
		tabs[k]='\t';

	tabs[cur_depth] = '\0';

	if(depth==cur_depth) {
		fprintf(input,"%s .inner-class\n",tabs);
		fprintf(output,"%s <div class=\"inner-class\"> \n %s</div> \n", tabs,tabs);
		return;
	}
	
	for(i=0; i<randomBranching; i++) {
		
		char *elem = types[0];
		char *attr = malloc(sizeof(char)*100);
		char *attr_input = malloc(sizeof(char)*100);
		if(rand()%2 == 0)
		{
			 sprintf(attr, "class=\"testclass_%d\" \0", cur_depth);
			 sprintf(attr_input,".testclass_%d \0", cur_depth);
		}
		else {

			 sprintf(attr, "id=\"testid_%d\"", cur_id);
			 sprintf(attr_input,"#testid_%d", cur_id);
			 cur_id++;
		}
		
		printf("%s<%s %s>\n",tabs,elem,attr);
		fprintf(output, "%s<%s %s>\n",tabs,elem,attr);
		fprintf(input, "%s %s {\n",tabs, attr_input);
		
    	printByDepth(depth,branch,cur_depth+1);
    	
    	printf("%s</%s>\n",tabs,elem);
    	fprintf(output, "%s</%s>\n",tabs,elem);
    	if(randomBranching == (i+1))
    	fprintf(input,"%s}\n",tabs);
    	else
    	fprintf(input,"%s},\n",tabs);
    }
}