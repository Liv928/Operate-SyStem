#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
 

/*
  Simple program that uses execl

*/

int main()
{

  int i = 5;
  int status;

  printf("%d\n",i);

  status = execl("external","external",NULL);

  if (status < 0)
    printf("\n main: execl failed");
    
 
 printf("%d\n",i);

}