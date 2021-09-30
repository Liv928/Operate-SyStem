#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>

/*  

   This program forks a process.  The child and parent processes print to terminal to identify themselves

*/

int main ()
{
pid_t i, j,  pid;

pid=fork();

if (pid <0) // fork unsuccessful
 {
	printf("fork unsuccessful");
	exit(1);
 }

if (pid>0) //parent
 {
	
 for (i=0; i< 10; i++)
	printf("I am Parent...iteration %d\n", i);
	
	wait(NULL);
		
 }

if (pid==0) //child
  {

	 for (i=0; i< 10; i++)
		printf("I am Child...iteration %d\n", i);
	
  }  
       
return 0;

}