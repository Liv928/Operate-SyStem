#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>

int main(void){
  int n;
  int status;
  char c;
  int fd[2];
  pid_t pid;
 
  if (pipe(fd) < 0){
    perror("pipe error");
    exit(0);
  } 

  pid = fork();
  if (pid < 0) {
    perror("fork error");
    exit(0);
  }

  if(pid > 0) //parent
  {
    printf("\n From parent: writing attempt #1 to pipe now..");
    write(fd[1],"ABC\n",4);
    printf("\n From parent: waiting for child to complete..\n");
    wait(NULL);
    printf("\n From Parent: reading attempt #1 from the pipe now..");
    read (fd[0],&c,1);
    printf("\n from parent: this is what I read %c\n", c);
    
  
  } else //child
   
  {
    printf("\n From Child: reading attempt #1 from the pipe now..");
    read (fd[0],&c,1);
    printf("\n from child: this is what I read %c", c);
    
    printf("\n From Child: reading attempt #2 from the pipe now..");
    read (fd[0],&c,1);
    printf("\n from child: this is what I read %c", c);

/*
    printf("\n From Child: reading attempt #3 from the pipe now..");
    read (fd[0],&c,1);
    printf("\n from child: this is what I read %c", c);


    printf("\n From Child: reading attempt #4 from the pipe now..");
    read (fd[0],&c,1);
    printf("\n from child: this is what I read %c", c);

*/


  }

  return 0;

}
