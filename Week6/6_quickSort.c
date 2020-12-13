#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <pthread.h>
#define MAX 1000

int isInteger(char* string);
int partition(int begin, int end);

// Define the array and length as gloabal variable
int arr[MAX];
int n;

// Define a struct to pass the positional arguments to the thread
struct Position
{
	int start;
	int end;
};

void *quickSort_thread(void *arg);

int main(int argc, char *argv[])
{

	if(argc < 2)
		printf("Insufficent arguments.\n");
	else 
	{
		int len = argc - 1;
		int i=1;
		for(;i<=len;++i)
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
			// Declare the main thread
			pthread_t tid;

			// Define the struct for the start and end positions to be passed
			struct Position *pos = (struct Position *)malloc(sizeof(struct Position));
			pos->start = 0;
			pos->end = n-1;

			// Create the main thread
			pthread_create(&tid,NULL,quickSort_thread,pos);

			// Join the array
			pthread_join(tid,NULL);

			// Print the array
			int i=0;
			printf("Sorted Array: ");
			for(;i<n;++i)
				printf("%d ",arr[i]);
			puts("");
		}
	}
	return 0;
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

// Define the runner function of the thread - mergeSort_thread
void *quickSort_thread(void *arg)
{
	int begin = ((struct Position*)arg)->start;
	int end = ((struct Position*)arg)->end;

	if(begin < end)
	{
		int p = partition(begin,end);

		// Create two threads - one for the LH partition and other for RH patition
		pthread_t tid[2];

		// Define the position struct for the Left Hand Half
		struct Position *leftPos = (struct Position *) malloc(sizeof(struct Position));
		leftPos->start = begin;
		leftPos->end = p-1;

		// Define the poisiton struct for the Right Hand Half
		struct Position *rightPos = (struct Position *)malloc(sizeof(struct Position));
		rightPos->start = p+1;
		rightPos->end = end;

		// Call the threads for both the halves
		pthread_create(&tid[0],NULL,quickSort_thread,leftPos);
		pthread_create(&tid[1],NULL,quickSort_thread,rightPos);

		// Join both the threads
		pthread_join(tid[0],NULL);
		pthread_join(tid[1],NULL);
	}

	// Exit the thread
	pthread_exit(NULL);
}

// QuickSort - Partition Function
int partition(int begin, int end)
{
	int pivot = arr[end];
	int i = begin - 1,j,temp;
	for(j= begin;j<=end-1;++j)
	{
		if(arr[j] < pivot)
		{
			i++;
			// swap
			temp = arr[i];
			arr[i] = arr[j];
			arr[j] = temp;
		}
	}

	temp = arr[i+1];
	arr[i+1] = arr[end];
	arr[end] = temp;

	return i + 1;
}