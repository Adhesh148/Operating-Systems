/******************************************
* AUTHOR : AdheshR *
* Problem Description: Dictionary of atleast 20 words*
******************************************/
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <inttypes.h>
#include <pthread.h>
#include <string.h>
#define MAX 4096
#define RANGE 100

#define BUFFER_SIZE 10				// indicates the size of buffer in a bounded-buffer setup 

// Define global variables to be accessed to and from.
struct dict
{
	char word[MAX];
	char pri_mean[MAX];
	char sec_mean[MAX];
};

int counter = 0;
int in = 0;
int out = 0;

// Define the buffer array
char *buffer[BUFFER_SIZE];

// Define variables for peterson algo
int flag[2];
int turn;

// Define a linear dictionary
int pos = 0;
struct dict dictionary[RANGE];

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
	struct dict nextProduced;

	/* -- Producer Code -- */
	while(1)
	{	
		while(counter == BUFFER_SIZE);			// buffer is full. wait for consumption

		/* -- Entry Section -- */
		flag[0] = 1,turn = 1;

		while(flag[1] == 1 && turn == 1);

		/* -- Critical Section -- */

		// Get word record from file
		memset(line,0,4096);
		if(fscanf(fp, "%[^\n]\n", line) == EOF)
		{
			flag[0] = 0;
			break;
		}

		// Split the dictionary record
		strcpy(nextProduced.word,strtok(line,";"));
		strcpy(nextProduced.pri_mean,strtok(NULL,";"));
		strcpy(nextProduced.sec_mean,strtok(NULL,"\n"));

		// Insert into dictionary
		printf("[PRODUCER] Word: %s\n",nextProduced.word);
		strcpy(dictionary[pos].word,nextProduced.word);
		printf("[PRODUCER] Primary meaning: %s\n",nextProduced.pri_mean);
		strcpy(dictionary[pos].pri_mean,nextProduced.pri_mean);
		printf("[PRODUCER] Secondary meaning: %s\n",nextProduced.sec_mean);
		strcpy(dictionary[pos++].sec_mean,nextProduced.sec_mean);

		// Produce next word
		buffer[in] = nextProduced.word;

		// Increment in pointer & increment counter
		in = (in + 1)%BUFFER_SIZE;
		counter++;

		/* -- Exit Section -- */
		flag[0] = 0;
		sleep(1);
	}
}

void *consumer(void *args)
{
	struct dict nextConsumed;

	/* -- Consumer Code -- */
	while(1)
	{
		while(counter == 0); 					// buffer is empty. wait for production

		/* -- Entry Section -- */
		flag[1] = 1, turn = 0;

		while(flag[0] == 1 && turn == 0);

		/* -- Critical Section -- */

		// Consume next student item
		strcpy(nextConsumed.word,buffer[out]);

		// Only the word gets passed from the producer to the consumer
		printf("[CONSUMER] Word: %s\n",nextConsumed.word);

		// Find the primary meaning and secondary meaning from the global dictionary - linear search
		int i=0;
		for(i=0;i<RANGE;++i)
		{
			if(strcmp(nextConsumed.word,dictionary[i].word) == 0)
			{
				strcpy(nextConsumed.pri_mean,dictionary[i].pri_mean);
				strcpy(nextConsumed.sec_mean,dictionary[i].sec_mean);
				break;
			}
		}
		
		printf("[CONSUMER] Primary meaning: %s\n",nextConsumed.pri_mean);
		printf("[CONSUMER] Secondary meaning: %s\n",nextConsumed.sec_mean);

		// Increment out pointer & decrement counter
		out = (out + 1)%BUFFER_SIZE;
		counter--;

		/* -- Exit Section -- */
		flag[1] = 0;
	}
}
