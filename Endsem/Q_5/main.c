/******************************************
* AUTHOR : AdheshR *
* Problem Description: Q5: Execute three binaries in threads using RR and PCS*
******************************************/
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>
#define MAX 100
#define NUM_THREADS 3

// Define the binaries to be executed
char *binaries[] = {"./binsearch","./matmul","./primegen"};

void *bin_runner(void *arg);

int main()
{
	int i,scope,policy;

	// define the threads
	pthread_t tid[NUM_THREADS];
	pthread_attr_t attr;

	// Get default attributes
	pthread_attr_init(&attr);

	// Inquire the scope
	if(pthread_attr_getscope(&attr, &scope) != 0)
		fprintf(stderr, "Unable to get scheduling scope.\n");
	else
	{
		if(scope == PTHREAD_SCOPE_PROCESS)
			printf("PTHREAD_SCOPE_PROCESS\n");
		else if(scope == PTHREAD_SCOPE_SYSTEM)
		{
			printf("PTHREAD_SCOPE_SYSTEM\n");
		}
		else
			fprintf(stderr, "Illegal scope value\n");
	}

	// Set scope to PCS if not already
	if(scope != PTHREAD_SCOPE_PROCESS)
		pthread_attr_setscope(&attr,PTHREAD_SCOPE_PROCESS);
	printf("scheduling scope set to PTHREAD_SCOPE_PROCESS\n");

	// Inquire the current scheduling policy
	if(pthread_attr_getschedpolicy(&attr,&policy) != 0)
		fprintf(stderr, "Unable to get the scheduling policy\n");
	else
	{
		if(policy == SCHED_OTHER)
			printf("SCHED_OTHER\n");
		else if(policy == SCHED_RR)
			printf("SCHED_RR\n");
		else if(policy == SCHED_FIFO)
			printf("SCHED_FIFO\n");
	}

	// Set scheduling policy to RR if not already
	if(policy != SCHED_RR)
	{
		// Set to RR
		if(pthread_attr_setschedpolicy(&attr,SCHED_RR) !=0)
			fprintf(stderr, "Unable to set scheduling policy\n");
		else
			printf("scheduling policy set to SCHED_RR\n");
	}

	// Create and call the threads
	for(i=0;i<NUM_THREADS;++i)
	{
		int *indx = malloc(sizeof(int));
		*indx = i;
		pthread_create(&tid[i],&attr, bin_runner,indx);
	}

	// Join all the threads after exit
	for(i=0;i<NUM_THREADS;++i)
		pthread_join(tid[i],NULL);

	return 0;
}

// Define the thread runners
void *bin_runner(void *arg)
{
	// Get the argument in appropriate type
	int indx = *((int *)arg);
	printf("%s\n",binaries[indx]);

	// Execute the appropriate binary using execlp call
	system(binaries[indx]);

	pthread_exit(NULL);
}