#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>

int main(){
	pid_t pid1,i,j;
	pid_t pid2;
	
	pid1 = fork();
	pid2 = fork();


	if(pid1<0){
		printf("fork unsuccessful ");	
		exit(1);
	}

	if(pid2<0){
		printf("fork unsuccessful ");	
		exit(1);
	}
	
	if(pid1>0&&pid2>0){//parent
		i= getpid();
		printf("\n Parent process(PID %d) created child_1(PID%d).",i,pid1);
      	
     
     	wait(NULL);
      
      	printf("\n Parent process(PID %d) created child_2(PID%d).",i,pid2);
      	printf("\n Child_2 (PID %d) is calling an external program external_program.out and leaving child_2… ",pid2);
	}

	if(pid1==0&&pid2>0){//child 1
		i= getpid();
		j= getppid();
		printf("\n Parent process(PID %d) is waiting for child_1(PID%d) to complete before creating child_2.",j,i);
		printf("\n Child_1(PID %d) created child_1.1 (PID %d).",i,pid2);
		wait(NULL);
		printf("Child_1(PID %d) is now completed.",i);
	}
	
	if(pid1>0&&pid2==0){//child2
		i= getpid();
		
      	execl("external_program.out", NULL); 
		
	}

   


}


