/******************************************
* AUTHOR : AdheshR *
* Problem Description: Implement the solution for the reader writer using semaphores.*
******************************************/
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <inttypes.h>
#include <pthread.h>
#include <semaphore.h>
#define MAX 100

// Define semaphores
sem_t mutex;
sem_t writeblock;

// Define shared variables
int data = 0, rcount = 0;

void *reader(void *arg);
void *writer(void *arg);

int main()
{
	int i;

	pthread_t rtid[5], wtid[5];

	// init the semaphore
	sem_init(&mutex,0,1);
	sem_init(&writeblock,0,1);

	for(i=0;i< 5;++i)
	{
		int *arg = malloc(sizeof(*arg));
		*arg = i; 
		pthread_create(&wtid[i], NULL, writer, arg);
		pthread_create(&rtid[i], NULL, reader, arg);
	}

	for(i=0;i< 5;++i)
	{
		pthread_join(wtid[i],NULL);
		pthread_join(rtid[i],NULL);
	}

	return 0;
}

// Define the reader thread runner
void *reader(void *arg)
{
	int f;
	f = *((int*) arg);

	sem_wait(&mutex);
	rcount = rcount + 1;

	if(rcount == 1)
		sem_wait(&writeblock);

	sem_post(&mutex);
	printf("Data read by reader %d is %d\n",f,data);
	sleep(1);

	sem_wait(&mutex);
	rcount = rcount - 1;

	if(rcount == 0)
		sem_post(&writeblock);

	sem_post(&mutex);
}

// Define writer runner
void *writer(void *arg)
{
	int f;
	f = *((int*) arg);

	sem_wait(&writeblock);
	data++;

	printf("Data written by writer %d is %d\n",f,data);
	sleep(1);

	sem_post(&writeblock);
}