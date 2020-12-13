#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <pthread.h>
#define MAX 1000

int isInteger(char* string);
void sortArray(int arr[],int n,int order);

// allows the array to be passed by value (not by reference)
struct arrayWrapper
{
	int arr[MAX];
	int len;
	int order;
};

void *runner(void *arg);

int main(int argc, char *argv[])
{	

	struct arrayWrapper *A = (struct arrayWrapper *)malloc(sizeof(struct arrayWrapper));


	if(argc < 2)
		printf("Insufficent arguments.\n");
	else 
	{
		int len = argc;
		int n=0, i=1;
		for(;i<len;++i)
		{
			// Validation function. Warning if not integer.
			if(isInteger(argv[i]))
				A->arr[n++] = atoi(argv[i]);
			else
				printf("warning: '%s' is not an integer. will be ignored.\n",argv[i]);
		}
		A->len = n;

		struct arrayWrapper B = *A;

		// Warning if entire argument list is invalid.s
		if(n == 0 )
			printf("The input list is invalid. Please enter an integer array.\n");
		else
		{
			/* -- Sort the array in two arrays in two threads
			      Create two threads -- */
			pthread_t tid[2];
			A->order = 1;
			pthread_create(&tid[0],NULL,runner,A);
			B.order = 0;
			pthread_create(&tid[1],NULL,runner,(void *)&B);

			// // Join both threads
			pthread_join(tid[0],NULL);
			pthread_join(tid[1],NULL);

			// print the results
			int i=0;
			printf("The sorted (ascending) output is:\n");
			for(;i<n;++i)
				printf("%d ",A->arr[i]);
			puts("");

			printf("The sorted (descending) output is:\n");
			i=0;
			for(;i<n;++i)
				printf("%d ",B.arr[i]);
			puts("");
		}
	}
	return 0;
}

// Runner function for thread
void *runner(void *arg)
{
	int *arr = ((struct arrayWrapper*)arg)->arr;
	int order = ((struct arrayWrapper*)arg)->order;
	int n = ((struct arrayWrapper*)arg)->len;

	/* -- Sort -- */

	// For swap purposes
	int temp;													

	// Initiate Loops for simple Bubble Sort.
	int i=0;
	for(;i<n;++i)
	{
		int j=0;
		for(;j<n-i-1;++j)
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
	pthread_exit(NULL);
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
