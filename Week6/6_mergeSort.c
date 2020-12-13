#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <pthread.h>
#define MAX 1000

int isInteger(char* string);
void merge(int begin, int mid, int end);

// Define the array and length as gloabal variable
int arr[MAX];
int n;

// Define a struct to pass the positional arguments to the thread
struct Position
{
	int start;
	int end;
};

void *mergeSort_thread(void *arg);

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
			pthread_create(&tid,NULL,mergeSort_thread,pos);

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
void *mergeSort_thread(void *arg)
{
	int begin = ((struct Position*)arg)->start;
	int end = ((struct Position*)arg)->end;

	if(begin < end)
	{
		int mid = begin + (end - begin)/2;

		// Create two threads - one for the LH partition and other for RH patition
		pthread_t tid[2];

		// Define the position struct for the Left Hand Half
		struct Position *leftPos = (struct Position *) malloc(sizeof(struct Position));
		leftPos->start = begin;
		leftPos->end = mid;

		// Define the poisiton struct for the Right Hand Half
		struct Position *rightPos = (struct Position *)malloc(sizeof(struct Position));
		rightPos->start = mid+1;
		rightPos->end = end;

		// Call the threads for both the halves
		pthread_create(&tid[0],NULL,mergeSort_thread,leftPos);
		pthread_create(&tid[1],NULL,mergeSort_thread,rightPos);

		// Join both the threads
		pthread_join(tid[0],NULL);
		pthread_join(tid[1],NULL);

		// call the merge function
		merge(begin,mid,end);
	}

	// Exit the thread
	pthread_exit(NULL);
}

// Define the merge function
void merge(int begin, int mid, int end)
{
	int lsize = mid - begin + 1;
	int rsize = end - mid;

	// create left and right arrays
	int left[lsize], right[rsize];

	// fill in the arrays
	int i=0,j=0,k=0;
	for(i=0;i<lsize;++i)
		left[i] = arr[begin + i];
	for(j=0;j<rsize;++j)
		right[j] = arr[mid+1+j];

	k = begin;
	i=0,j=0;

	while(i<lsize && j<rsize)
	{
		if(left[i] < right[j])
			arr[k] = left[i++];
		else
			arr[k] = right[j++];

		++k;
	}

	// Get the remaining elements from both the arrays into the merged array
	while(i<lsize)
		arr[k++] = left[i++];

	while(j<rsize)
		arr[k++] = right[j++];
}