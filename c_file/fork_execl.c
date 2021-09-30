#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>

/*
  Use of fork and exec
*/

int main()
{

  pid_t pid;
  int status;

  pid = fork();
  if (pid < 0){
   perror("Fork()");
   printf("errno is %d\n", errno); 
   exit(pid);
  }

  if (pid > 0)
  {
     wait(NULL);
     printf("child completed \n");
  } 
  
  if (pid == 0)
  {
	status = execl("external", "external", NULL);
      	
	if (status < 0)
	{
        perror("exec");
        printf("main: errno is %d\n",errno);        
      	}
	
	printf("\n From Child...returning to parent");

 } 
   
return 0;

}
