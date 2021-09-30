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
pid_t i, j, pid;

pid=fork();

if (pid <0) // fork unsuccessful
 {
	printf("fork unsuccessful");
	exit(1);
 }

if (pid>0) //parent
 {
	i = getpid();
	wait(NULL);
	printf("\n I am parent with PID %d\n", i);
	
 }

if (pid==0) //child
  {
	i = getpid();
   	j = getppid();
   	printf("\n I am a child with PID %d and my parent’s PID is %d\n", i, j);
  }  
       
}