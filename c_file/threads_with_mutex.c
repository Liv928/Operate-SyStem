#include<stdio.h>
#include<string.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define withdraw_req 400
#define NUM_THREADS 5

void  *withdraw_money(void *thread_id);

int shared_balance = 1000;

pthread_mutex_t lock;



void *withdraw_money(void *thread_id)
{

long int *temp = (long int *) thread_id;
long int thread_name = *temp;


printf("\n\n ****From withdraw_money thread ID %ld****", thread_name);

pthread_mutex_lock(&lock);  // ENTRY REGION


printf("\n current balance is %d", shared_balance); //CRITICAL STARTS
printf("\n Processing Thread ID %ld request for $ %d", thread_name, withdraw_req);


if (shared_balance >= withdraw_req)
{
	printf("\n Withdraw amount of %d is Authorized", withdraw_req);
	for (int i = 0; i<1000000;i++);
	shared_balance = shared_balance - withdraw_req;
	printf("\n Updated balance %d\n", shared_balance); 

}

else
	
	printf("\n Request denied"); //CRITICAL ENDS
	


pthread_mutex_unlock(&lock); // EXIT REGION

}



int main(void)
{
    
int i, err_thread;

pthread_t threads[NUM_THREADS];

if (pthread_mutex_init(&lock, NULL) != 0)
    {
        printf("\n mutex init failed\n");
        return 1;
    }  


for (i = 0; i< NUM_THREADS; i++)
  {
    err_thread = pthread_create(&threads[i], NULL, &withdraw_money, &threads[i]); 

if (err_thread != 0)
	printf("\n Error creating thread %d", i);
}


for (i = 0; i< NUM_THREADS; i++)
	pthread_join(threads[i], NULL);

pthread_mutex_destroy(&lock); 
 

return 0;

}

