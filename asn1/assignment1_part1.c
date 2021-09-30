/*
*@author Yu Wu ywu928
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>

int main(){
	pid_t pid1,i,j;
	pid_t pid2,x,y;
	
	pid1 = fork();//create child_1
	
	if(pid1<0){
		printf("fork unsuccessful ");	
		exit(1);
	}

	
	if(pid1>0){//parent
		j= getpid();
	  	wait(NULL);//wait child_1 to complete
		pid2 = fork();//after child_1 complete, create child_2 
		
		if(pid2<0){
			printf("fork unsuccessful ");	
			exit(1);
		}

		if(pid2==0){//child_2
			i= getpid();
			
			printf("Parent process(PID %d) created child_2(PID %d).\n",j,i);
			printf("Child_2 (PID %d) is calling an external program external_program.out and leaving child_2… \n",i);
			execl("external_program.out", NULL); 		
		}
		
	}

	if(pid1==0){//child_1
		i=getpid();
		j=getppid();
		printf("Parent process(PID %d) created child_1(PID %d).\n",j,i);
      	printf("Parent process(PID %d) is waiting for child_1(PID %d) to complete before creating child_2.\n",j,i);
		pid2=fork();//create child_1.1
		
		if(pid2<0){
			printf("fork unsuccessful ");	
			exit(1);
		}
		if(pid2==0){//child_1.1
			x=getpid();
			y=getppid();
			printf("Child_1(PID %d) created child_1.1 (PID %d).\n",y,x);
			printf("Child_1(PID %d) is now completed.\n",i);	
		}
		
	}
	

	
}


