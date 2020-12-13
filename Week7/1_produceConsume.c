/******************************************
* AUTHOR : AdheshR *
* Problem Description: Implement the Producer-Consumer code where one thread produces some data whereas another thread consumes it. 
						The implementation will illustrate the possible fallacies that can arrive due to asynchronous access of data and 
						re-iterate the need for a synchronous data access in a multi-threaded setup.*
* Remark: This producer consumer works fine as no concurrency seems to happen between ++ (increment) or -- (decrement). They seem to happen atomically. To see the failing, maybe
			try to do the increment in steps and sleep in between.
******************************************/
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <inttypes.h>
#include <pthread.h>
#include <string.h>
#define MAX 4096

#define BUFFER_SIZE 10				// indicates the size of buffer in a bounded-buffer setup 

// Define global variables to be accessed to and from.
struct student
{
	char rollNo[MAX];
	char name[MAX];
};


int counter = 0;
int in = 0;
int out = 0;

// Define the buffer array
struct student buffer[BUFFER_SIZE];

void *producer(void *args);
void *consumer(void *args);

int main(int argc, char *argv[])
{
	/*-- Main accepts the filename of student record as input argument from command line
		 Validate the arguments -- */

	if(argc!=2)
	{
		printf("Invalid arguments.\n");
		exit(EXIT_FAILURE);
	}

	// Validate the file
	FILE *fp;
	if((fp = fopen(argv[1],"r")) == NULL)
	{
		printf("File does not exist.\n");
		exit(EXIT_FAILURE);
	}

	// Create threads for the producer and consumer
	pthread_t tid[2];

	// Create and init attr to default
	pthread_attr_t attr;
	pthread_attr_init(&attr);

	// Call the producer thread and also pass the file name
	pthread_create(&tid[0],&attr,producer,argv[1]);

	// Call the consumer thread
	pthread_create(&tid[1],&attr,consumer,NULL);

	pthread_join(tid[0],NULL);
	pthread_join(tid[1],NULL);

	return 0;
}

void *producer(void *args)
{	
	// extract file name from args
	char filename[MAX];
	strcpy(filename,(char *)args);

	// Open file to be read
	FILE *fp = fopen(filename,"r");
	char line[4096];
	int len = 0;

    // Define struct for nextProduced student record
	struct student nextProduced;

	/* -- Producer Code -- */
	while(1)
	{	
		while(counter == BUFFER_SIZE);			// buffer is full. wait for consumption

		// Get student data from file
		memset(line,0,4096);
		if(fscanf(fp, "%[^\n]\n", line) == EOF)
			break;

		// Split the record to name and rollno
		strcpy(nextProduced.rollNo,strtok(line," "));
		strcpy(nextProduced.name,strtok(NULL,"\n"));

		// Produce next student item
		buffer[in] = nextProduced;

		// Increment in pointer & increment counter
		in = (in + 1)%BUFFER_SIZE;
		counter++;
	}
}

void *consumer(void *args)
{
	struct student nextConsumed;

	/* -- Consumer Code -- */
	while(1)
	{
		while(counter == 0); 					// buffer is empty. wait for production

		// Consume next student item
		nextConsumed =  buffer[out];

		// Print data in consumed data
		printf("[CONSUMER] Roll No: %s\n",nextConsumed.rollNo);
		printf("[CONSUMER] Name: %s\n",nextConsumed.name);

		// Increment out pointer & decrement counter
		out = (out + 1)%BUFFER_SIZE;

		counter--;
	}
}
