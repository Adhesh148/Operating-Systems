/******************************************
* AUTHOR : AdheshR *
* Problem Description: *
******************************************/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <pthread.h>
#define MAX 1000

int isInteger(char* string);

// Define the array and length as gloabal variable
int arr[MAX];
int n;

// Define split parameters
int num_splits = 4;

// Define parameters for the search
int x;
int count = 0;

int main(int argc, char *argv[])
{

	if(argc < 2)
		printf("Insufficent arguments.\n");
	else 
	{
		int len = argc - 1;
		x = atoi(argv[len]);
		int i=1;
		for(;i<len;++i)
		{
		// Validation function. Warning if not integer.
		if(isInteger(argv[i]))
			arr[n++] = atoi(argv[i]);
		else
			printf("warning: '%s' is not an integer. will be ignored.\n",argv[i]);
		}

		// Warning if entire argument list is invalid.s
		if(n == 0 )
			printf("The input list is invalid. Please enter an integer array.\n");
		else
		{
			/* -- Search for x in ? splits using Binary Search -- */
			if(n < num_splits)
				num_splits = 2;

			if(n < num_splits)
				num_splits = 1;

			// Declare the threads
			pthread_t tid[num_splits];
			pthread_attr_t attr;

			// Init attr with default values
			pthread_attr_init(&attr);

			int split_no = 0;
			int split_size = n/num_splits;

			for(i=0;i<num_splits;++i)
			{
				int *arg = malloc(sizeof(*arg));
				*arg = split_no;
				pthread_create(&tid[i],&attr,exp_runner,arg);
				++split_no;
			}

			for(i=0;i<num_splits;++i)
				pthread_join(tid[i],NULL);

			printf("%d is present %d times in the given array\n",x,count);		
		}
	}
	return 0;
}

void *exp_runner(void *arg)
{
	int split_no = *((int *) arg);
	int start = (arr_len * split_no)/num_splits;
	int end = (arr_len * (split_no + 1))/num_splits;

	sortArray(arr,start,end,1);

	/* -- Perform Exponential Search -- */
	if(arr[start] == x)
}

void sortArray(int arr[],int start,int end,int order)
{
	int temp;													// For swap purposes

	// Initiate Loops for simple Bubble Sort.
	int i=start;
	int n = end - start;
	for(;i<end;++i)
	{
		int j=start;
		for(;j<start+end-i-1;++j)
		{
			if(((order == 1) && (arr[j]>arr[j+1])) || ((order == 0) && (arr[j]<arr[j+1])))
			{
				// Swap arr[j] and arr[j+1]
				temp = arr[j];
				arr[j] = arr[j+1];
				arr[j+1] = temp;
			}
		}
	}
}

// Validation Function
int isInteger(char* str)
{
	int character,i=0;
	for(;i<strlen(str);++i)
	{
		character = str[i];
		// In case of '-'' symbol, continue if not last character in string.
		if(character == 45 && i<strlen(str)-1)
			continue;
		// Else get the decimal value (-48) and check condition.
		character -= 48;
		if(character < 0 || character > 9)
			return 0;
	}
	return 1;
}