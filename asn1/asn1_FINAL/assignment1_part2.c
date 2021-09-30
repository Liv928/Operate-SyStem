#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>

int main(int argc, char *argv[]){
  int x,y,z;
  int status;
  char c[10];
  int fd[2];
  pid_t pid,i,j;
 
 // x=atoi(argv[1]);
 // y=atoi(argv[2]);
 // printf("argv[2] is %d",y);
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
    i=getpid();
    printf("\n A pipe is created for communication between parent (PID %d) and child.",i);
    write(fd[1],argv[1],2);
    read (fd[0],&c,2);
    x=atoi(c);
    printf("\n parent (PID %d) Reading X = %d from the user \n",i,x);
    wait(NULL);
    printf("Back to Parent!");
    read (fd[0],&c,2);
    y=atoi(c);
    printf("\n parent (PID %d) Reading Y from the pipe (Y = %d)",i,y);
    printf("\n parent (PID %d) adding X + Y = %d",i,x+y);
  
  } else //child
   
  {
    i=getpid();
    j=getppid();
    printf("\n parent (PID %d) created a child (PID %d). ",j,i);
    
    write(fd[1],argv[2],2);
    read (fd[0],&c,2);
    z=atoi(c);
    printf("\n child (PID %d) Reading Y = %d from the user ",i,z);
    write(fd[1],c,2);
    printf("\n child (PID %d) Writing Y into the pipe ",i);
    
  }

  return 0;

}
