#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

int interrupt_status = 0;

void signal_handler()
{

printf("\n SIGINT Signal: User pressed ctr+c...\n");

interrupt_status = 1;
}


int main(void)
{

signal(SIGINT, signal_handler);

while(interrupt_status == 0)
{

//program waiting for user to press "ctr+c"...


} 

printf ("\n back from signal_handler");

return 0;
}