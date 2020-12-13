/******************************************
* AUTHOR : AdheshR *
* Problem Description: *
******************************************/
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <inttypes.h>
#include <pthread.h>
#include <semaphore.h>
#define MAX 100

// Define shared variables
int riders = 0;

// Define the semaphores
sem_t mutex;
sem_t multiplex;
sem_t bus;
sem_t allAboard;

// Define global const parameters
static const int N = 60;

// Declare functions
void *bus_runner(void *args);
void *rider_runner(void *args);

void boardBus(int rider_num);
void depart();

int main()
{
	// Loop variable
	int i = 0;

	// Init the semaphore variables
	sem_init(&mutex,0,1);
	sem_init(&multiplex,0,50);
	sem_init(&bus,0,0);
	sem_init(&allAboard,0,0);

	// Create and run the bus thread
	pthread_t bus_tid;
	pthread_create(&bus_tid,NULL,bus_runner,NULL);

	// Create and run the rider threads
	pthread_t rider_tid[N];
	for(i=0;i<N;++i)
	{
		int *arg = malloc(sizeof(*arg));
		*arg = i + 1; 
		pthread_create(&rider_tid[i],NULL,rider_runner,arg);
	}

	// Join all threads
	pthread_join(bus_tid,NULL);

	for(i=0;i<N;++i)
		pthread_join(rider_tid[i],NULL);

	return 0;
}

// Define the bus runner
void *bus_runner(void *args)
{
	while(1)
	{
		sem_wait(&mutex);
		if(riders > 0)
		{
			sem_post(&bus);
			sem_wait(&allAboard);
		}
		sem_post(&mutex);

		depart();
	}
}

// Define the rider runner
void *rider_runner(void *args)
{
	int rider_num = *(int*)args;
	while(1)
	{
		sem_wait(&multiplex);
		sem_wait(&mutex);
		++riders;
		printf("[RIDER] No of waiting riders = %d\n",riders);
		sem_post(&mutex);

		sem_wait(&bus);
		sem_post(&multiplex);

		boardBus(rider_num);

		--riders;
		if(riders == 0)
			sem_post(&allAboard);
		else
			sem_post(&bus);
	}
}

// Define utility functions
void boardBus(int rider_num)
{
	printf("[RIDER] Bus has arrived. Rider %d has boarded.\nNo of riders waiting = %d\n",rider_num,riders);
	sleep(1);
}

void depart()
{
	printf("[BUS] Departing. Riders waiting = %d\n",riders);
	sleep(1);
}