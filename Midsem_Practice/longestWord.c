/******************************************
* AUTHOR : AdheshR *
* Problem Description: *
******************************************/
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <inttypes.h>
#include <string.h>
#include <fcntl.h>
#define MAX 4096

// Define gloabal variables
long int num_bytes = 0;
int max_len = 0;
char longestWord[MAX];
char file_name[MAX];

// Define thread parameters
int num_splits = 4;

struct fileArgs
{
	int split_no;
};

void *longestWord_runner(void *arg);

int main(int argc, char *argv[])
{
	// Validate input
	if(argc <= 1)
	{
		printf("Insufficient arguments passed.\n");
		exit(EXIT_FAILURE);
	}
	else if(argc == 2)
	{
		// Get file name
		strcpy(file_name,argv[1]);
		printf("[FILE IO] File Name: %s\n",file_name);

		// Open File 
		FILE *fp;
		fp = fopen(file_name,"r");
		if(fp == NULL)
		{
			printf("[FILE IO] Unable to open file - %s\n",file_name);
			exit(EXIT_FAILURE);
		}

		// Get the size of the file in bytes
		fseek(fp, 0, SEEK_END);
		num_bytes = ftell(fp);
		printf("%ld\n",num_bytes);

		// Declare the threads
		pthread_t tid[num_splits];
		pthread_attr_t attr;
		// Init attr with default values
		pthread_attr_init(&attr);
		
		struct fileArgs param[num_splits];

		int i;
		for(i=0;i<num_splits;++i)
		{
			param[i].split_no = i;
			pthread_create(&tid[i],&attr,longestWord_runner,(void *)&param[i]);
		}

		for(i=0;i<num_splits;++i)
			pthread_join(tid[i],NULL);

		printf("Longest Word = %s\n",longestWord);
		printf("Length = %d\n",max_len);
	}
	return 0;
}

void *longestWord_runner(void *arg)
{
	struct fileArgs *param = (struct fileArgs*)arg;
	int split_no = param->split_no;
	long int start = (num_bytes * split_no)/num_splits;
	long int end = (num_bytes * (split_no + 1))/num_splits;

	FILE *fp;
	fp = fopen(file_name,"r");

	// Seek to starting byte position
	fseek(fp,start,SEEK_CUR);

	char c;
	char currWord[MAX];
	memset(currWord,0,MAX);
	int curr_len = 0;
	while(ftell(fp)!=end)
	{
		c = fgetc(fp);
		if(c == ' ' || c == '\n' || c == '.' || c == ',')
		{
			currWord[curr_len] = '\0';
			if(curr_len > max_len)
			{
				max_len = curr_len;
				strcpy(longestWord,currWord);
			}
			curr_len = 0;
			memset(currWord,0,MAX);
		}
		else
		{
			currWord[curr_len] = c;
			++curr_len;
		}
	}

	// Boundary condition
	if(curr_len!=0)
	{
		while(1)
		{
			c = fgetc(fp);
			if(c == ' ' || c == '\n' || c == '.' || c == ',')
			{
				currWord[curr_len] = '\0';
				if(curr_len > max_len)
				{
					max_len = curr_len;
					strcpy(longestWord,currWord);
				}
				curr_len = 0;
				memset(currWord,0,MAX);
				break;
			}
			else
			{
				currWord[curr_len] = c;
				++curr_len;
			}
		}
	}
}