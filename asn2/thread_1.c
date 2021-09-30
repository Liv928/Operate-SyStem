#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h>   
#include <pthread.h> 
  


void *thread_prints_msg(void *msg) 
{ 
    printf("From thread_prints_msg: %s\n", (char *) msg); 
    return 0; 
} 
   

int main() 
{ 
    pthread_t thread_1; 
    printf("From main: Going to create Thread...\n");     
    pthread_create(&thread_1, NULL, thread_prints_msg, "Hello World"); 
    pthread_join(thread_1, NULL); 
    printf("thread terminates...\n"); 
    
return 0; 

}