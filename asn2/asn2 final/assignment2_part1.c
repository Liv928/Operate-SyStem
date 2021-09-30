/*
*@author Yu Wu ywu928
*assignment2_part1
*/
#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h>   
#include <pthread.h> 
#include <sys/wait.h>
typedef struct t_para{
    int x1;
    int y1;
    int z1;
}t_para;

void *thread_sums_two_integers(void *data) 
{ 
	t_para *fun_para;
    fun_para = (t_para *) data;
    fun_para->z1 = fun_para->x1 +fun_para->y1;
    
    printf("From Thread: z is %d\n", fun_para->z1);
 
    return 0; 
} 
   

int main() 
{ 
   int x = 10, y = 20, z = 0;
   pid_t pid;
   pthread_t thread;
   int fd[2];
   char buf[10]={""};
   
   if (pipe(fd) < 0){
    perror("pipe error");
    exit(0);
   }

   pid = fork();

   if(pid<0){
       printf("fork unsuccessful.");
   }

   if(pid>0){
       
       wait(NULL);
       
       read(fd[0],buf,10);
       z=atoi(buf);
       printf("Child process completed. z is %d", z);
       printf("\n");
       printf("From main: Going to create Threads...\n");
       t_para mypara;
       mypara.x1=x;
       mypara.y1=y;
       mypara.z1=z;
       pthread_create(&thread, NULL, thread_sums_two_integers,&mypara);
       pthread_join(thread, NULL);
       z=mypara.z1;
       printf("Thread terminated. From parent: z is %d\n",z); 
   }
   
   if(pid==0){
       z=x+y;
       sprintf(buf,"%d",z);
       write(fd[1],buf,10);
       
   }
   return 0; 

}
