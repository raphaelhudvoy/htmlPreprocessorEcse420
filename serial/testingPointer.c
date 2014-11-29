
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

// main program
int main () {

 int threadID;

 // let's print something
 printf("\nI'm the initial thread.\n");

 int i = 0;
 int b = 2, c = 1;

 if (b == 2) {
 	c = 2;
 } else {
 	c = 3;
 }

 // initial (master) thread creates 3 more threads
 #pragma omp parallel num_threads(c)
 {
   // Get thread number
   threadID = omp_get_thread_num();
   i++;
   printf("\tSection 1 - I'm thread %d with i = %d\n", threadID, i);
 } // All threads join master thread and disband


 printf("\nMaster thread - Here I am again\n");

 // set a new number of threads
 omp_set_num_threads(3);
 #pragma omp parallel 
 {
   // Get thread number
   threadID = omp_get_thread_num();
   printf("\tSection 2 - I'm thread %d\n", threadID);
 } // All threads join master thread and disband

 return 0;
}
