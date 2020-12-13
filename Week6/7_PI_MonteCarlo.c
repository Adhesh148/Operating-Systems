#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <inttypes.h>
#include <pthread.h>

// Let us define program MACROS
#define INTERVAL 20000

// define global variables
long int num_circle_points = 0, num_sqr_points = 0;
double PI;

void *runner(void *arg);

int main()
{
	// Initialize the random number generator
	srand(time(NULL));

	// Define threads
	pthread_t tid[INTERVAL];

	// Loop 
	int i = 0;
	for(;i<(INTERVAL);++i)
		pthread_create(&tid[i],NULL,runner,NULL);
	
	// Join all threads back
	i = 0;
	for(;i<(INTERVAL);++i)
		pthread_join(tid[i],NULL);

	// Print the value of PI
	PI  = (double)(4 * num_circle_points) / (double)num_sqr_points;
	printf("PI = %lf\n",PI);

	return 0;
}

void *runner(void *arg)
{
	double random_x, random_y;
	random_x = (double)(rand() % (INTERVAL + 1)) / (double)INTERVAL;
	random_y = (double)(rand() % (INTERVAL + 1)) / (double)INTERVAL;

	double d = random_x * random_x + random_y * random_y;

	if(d <= 1)
		++num_circle_points;

	++num_sqr_points;
	
	// Exit thread
	pthread_exit(NULL); 
}
