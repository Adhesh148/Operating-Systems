/******************************************
* AUTHOR : AdheshR *
* Problem Description: Priority Inheritance solution implemented along with Bounded Wait TSL solution for n process - Here in the bounded wait condition check any process first unlocks
									highest priority process if it wants to enter the CS. Else it unlocks any other waiting process. If no process is waiting it just releases the lock*
******************************************/
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <inttypes.h>
#include <fcntl.h>
#define MAX 100
#define NUM_THREADS 3
#define L_PRIORITY 1
#define M_PRIORITY 2
#define H_PRIORITY 3

// Define the priority for three processes
int priority[] = {L_PRIORITY,M_PRIORITY,H_PRIORITY};

// Define the shared variable to access in CS
int data = 0;
int key = 0;
int lock = 0;
int waiting[] = {0,0,0};

int TestNSet(int *target);
void *runner(void *arg);

int main()
{
	int i;

	// Define three threads - L, M, H
	pthread_t tid[NUM_THREADS];

	// create threads and call runners
	for(i=0;i<NUM_THREADS;++i)
	{
		int *indx = malloc(sizeof(int));
		*indx = i;
		pthread_create(&tid[i],NULL,runner,indx);
	}

	// Join all threads
	for(i=0;i<NUM_THREADS;++i)
		pthread_join(tid[i],NULL);

	return 0;
}

// Define the thread to access CS
void *runner(void *arg)
{
	// Get the argument in appropriate type
	int indx = *((int *)arg);
	int j;

	while(1)
	{
		printf("\nProcess:%d is WAITING...\n",indx+1);

		/* -------- Entry Section ----------- */
		waiting[indx] = 1;
		key = 1;
		while(waiting[indx] && key)
			key = TestNSet(&lock);
		waiting[indx] = 0;

		/* ---------- Critical section -------------- */
		printf("\n%d\n",priority[indx]);
		printf("Process:%d is CRITICAL SECTION...\n",indx+1);
		data++;
		printf("Updated Data: %d\n",data);

		/* ---------- Exit section - Is based on allowing higher priority wait run first-------------- */
		j = (indx + 1)%NUM_THREADS;
		
		if(indx == 0)
		{
			if(waiting[2] == 1)
				waiting[2] = 0;
			else if(waiting[1] == 1)
				waiting[1] = 0;
			else
				lock = 0;
		}
		else if(indx == 1)
		{
			if(waiting[2] == 1)
				waiting[2] = 0;
			else if(waiting[0] == 1)
				waiting[0] = 0;
			else
				lock = 0;
		}
		else if(indx == 2)
		{
			if(waiting[1] == 1)
				waiting[1] = 0;
			else if(waiting[0] == 1)
				waiting[0] = 0;
			else
				lock = 0;
		}

		// Remainder section
		sleep(4);
	}

}

// Test N Set 
int TestNSet(int *target)
{
	int rv = *target;
	*target = 1;
	return rv;
}