/******************************************
* AUTHOR : AdheshR *
* Problem Description: Implement the solution for the dining philosopher problem using semaphores. Include Priority Inheritance in Dining Philosopher Problem. We can include it in the Test() function
						where a philosopher tests to see if the neighbors are not eating but hungry and have higher priority - forefit chance to give way to higher phil*
******************************************/
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <inttypes.h>
#include <pthread.h>
#include <semaphore.h>
#define MAX 100

// Define problem macros
#define N 5
#define THINKING 0
#define HUNGRY 1
#define EATING 2

#define LEFT (phnum + 4)%N
#define RIGHT (phnum + 1)%N

// Define sempahores
sem_t mutex;
sem_t S[N];

// Define shared variables
int state[N];
int phil_num[N] = {0,1,2,3,4};

// Define the priority of each philosopher
int priority[] = {1,2,3,4,5};

void *philosopher(void *num);
void take_fork(int phnum);
void put_fork(int phnum);
void test(int phnum);

int main()
{
	int i;

	pthread_t thread_id[N];

	// Init the semaphore
	sem_init(&mutex, 0,1);

	for(i=0;i<N;++i)
		sem_init(&S[i],0,0);

	// create and call thread runners
	for(i=0;i<N;++i)
	{
		pthread_create(&thread_id[i], NULL, philosopher, &phil_num[i]);
		printf("Philosopher %d is thinking.\n",i+1);
	}

	for(i=0;i<N;++i)
		pthread_join(thread_id[i],NULL);	
}

void *philosopher(void *num)
{
	while(1)
	{
		int *i = num;
		sleep(1);
		take_fork(*i);
		sleep(0);
		put_fork(*i);
	}
}

// Take chopsticks up
void take_fork(int phnum)
{
	sem_wait(&mutex);

	// move to HUNGRY state
	state[phnum] = HUNGRY;
	printf("Philosopher %d is hungry.\n",phnum);

	// Eait if neighbors arent eating
	test(phnum);

	sem_post(&mutex);

	// If unable to eat wait
	sem_wait(&S[phnum]);

	sleep(1);
}

// Put down chopstick
void put_fork(int phnum)
{
	sem_wait(&mutex);

	// move to state of THINKING
	state[phnum] = THINKING;
	printf("Philosopher %d putting fork %d and %d down\n",phnum + 1, LEFT + 1, phnum + 1); 
    printf("Philosopher %d is thinking\n", phnum + 1);

    test(LEFT); 
    test(RIGHT); 
  
    sem_post(&mutex); 
}

void test(int phnum) 
{ 
    if (state[phnum] == HUNGRY && state[LEFT] != EATING && state[RIGHT] != EATING) 
    { 
    	// Additional priority check - check if the left neighbor or the right neighbor is HUNGRY and has a higher priorty - then forefit turn to EAT
    	if((priority[LEFT] > priority[phnum - 1] && state[LEFT] == HUNGRY) || (priority[RIGHT] > priority[phnum - 1] && state[RIGHT] == HUNGRY))
    		sem_post(&S[phnum]);
    	else
    	{
    		// state that eating 
	        state[phnum] = EATING; 
	  
	        sleep(2); 
	  
	        printf("Philosopher %d takes fork %d and %d\n", phnum + 1, LEFT + 1, phnum + 1); 
	        printf("Philosopher %d is Eating\n", phnum + 1); 
	  
	        sem_post(&S[phnum]); 
    	} 
    } 
} 
