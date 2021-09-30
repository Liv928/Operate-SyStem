#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h>   
#include <pthread.h> 

  
int sum = 0;

void *thread_sums_two_integers (void *num) 
{ 
    
	int *number_to_be_added = (int*) num;

	int thread_id;

	if (*number_to_be_added == 10)
		thread_id = 1;
	else if (*number_to_be_added == 20)
		thread_id = 2;
	else if (*number_to_be_added == 30)
		thread_id = 3;
	else if (*number_to_be_added == 40)
		thread_id = 4;

	sum += *number_to_be_added;

    printf("From Thread %d: Sum is %d\n", thread_id, sum);
 
    return 0; 
} 
   

int main() 
{ 
   int i = 10, j = 20, k = 30, l = 40;

    pthread_t thread_1, thread_2,thread_3, thread_4;
 
    printf("From main: Going to create Threads...\n");
	
    pthread_create(&thread_1, NULL, thread_sums_two_integers, &i);
	
    pthread_create(&thread_2, NULL, thread_sums_two_integers, &j);
		
    pthread_create(&thread_3, NULL, thread_sums_two_integers, &k);
	
    pthread_create(&thread_4, NULL,thread_sums_two_integers, &l);
	
	pthread_join(thread_1, NULL);
	pthread_join(thread_2, NULL);
	pthread_join(thread_3, NULL);
	pthread_join(thread_4, NULL);

	printf("All threasd terminated...\n"); 
    
return 0; 

}