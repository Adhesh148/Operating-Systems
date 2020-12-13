/******************************************
* AUTHOR : AdheshR *
* Problem Description: Implement the Santa Claus Problem using semaphores*
******************************************/
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <inttypes.h>
#include <pthread.h>
#include <semaphore.h>

// Define const parameters
static const int N_REINDEERS = 9;
static const int N_ELVES = 10;

// Define shared variables
int elves = 0;
int reindeer = 0;

// Define semaphores
sem_t santaSem;
sem_t reindeerSem;
sem_t elfTex;
sem_t mutex;

// Pre-declare functions
void *elfRunner(void *args);
void *reindeerRunner(void *args);
void *santaRunner(void *args);

void prepareSleigh();
void helpElves();
void getHitched();
void getHelp();

int main()
{
	// Loop variable
	int i=0;
	

	// Init the semaphore variables
	sem_init(&santaSem,0,0);
	sem_init(&reindeerSem,0,0);
	sem_init(&elfTex,0,1);
	sem_init(&mutex,0,1);

	// Create and run a Santa thread
	pthread_t santa_tid;
	pthread_create(&santa_tid,NULL,santaRunner,NULL);

	// Create and run N_ELVES thread
	pthread_t elf_tid[N_ELVES];
	for(i=0;i<N_ELVES;++i)
	{
		int *arg = malloc(sizeof(*arg));
		*arg = i + 1; 
		pthread_create(&elf_tid[i],NULL,elfRunner,arg);
	}

	// Create and run the reindeer threads
	pthread_t reindeer_tid[N_REINDEERS];
	for(i=0;i<N_REINDEERS;++i)
	{
		int *arg = malloc(sizeof(*arg));
		*arg = i + 1; 
		pthread_create(&reindeer_tid[i],NULL,reindeerRunner,arg);
	}

	// Join all threads
	pthread_join(santa_tid,NULL);

	for(i=0;i<N_ELVES;++i)
		pthread_join(elf_tid[i],NULL);

	for(i=0;i<N_REINDEERS;++i)
		pthread_join(reindeer_tid[i],NULL);

	return 0;
}

// Define the santa runner thread
void *santaRunner(void *args)
{
	int i = 0;
	printf("[Santa] Santa is Here !!!\n");
	while(1)
	{
		sem_wait(&santaSem);
		sem_wait(&mutex);

		if(reindeer == 9)
		{
			prepareSleigh();
			for(i=0;i<9;++i)
				sem_post(&reindeerSem);
			printf("[Santa] Make kids happy\n");
			reindeer = 0;
		}
		else if(elves == 3)
			helpElves();

		sem_post(&mutex);
	}
}

// Define the reindeer runner thread
void *reindeerRunner(void *args)
{
	int reindeer_num = *(int*)args;
	while(1)
	{
		sem_wait(&mutex);
		reindeer++;
		if(reindeer == 9)
			sem_post(&santaSem);
		sem_post(&mutex);

		sem_wait(&reindeerSem);
		getHitched(reindeer_num);
		sleep(5);
	}
}

// Define the elf runner threads
void *elfRunner(void *args)
{
	int elf_num = *(int*)args;
	while(1)
	{
		sem_wait(&elfTex);
		sem_wait(&mutex);

		elves++;
		if(elves == 3)
			sem_post(&santaSem);
		else
			sem_post(&elfTex);

		sem_post(&mutex);

		getHelp(elf_num);

		sem_wait(&mutex);
		elves--;
		if(elves == 0)
			sem_post(&elfTex);
		sem_post(&mutex);
	}

	// Work a while
	printf("[Elf] Elf %d is working.\n",elf_num);
	sleep(3);
}

// Simple utility functions
void helpElves()
{
	printf("[Santa] Helping the elves.\n");
}

void prepareSleigh()
{
	printf("[Santa] Preparing Sleigh\n");
}

void getHelp(int elf_num)
{
	printf("[Elf] Elf %d needs help from Santa\n",elf_num);
	sleep(5);
}

void getHitched(int reindeer_num)
{
	printf("[Reindeer] Reindeer %d getting hitched.\n",reindeer_num);
	sleep(10);
}