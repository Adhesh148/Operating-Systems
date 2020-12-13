#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <math.h>
#include <pthread.h>
#define MAX 1000

int num_splits = 4;

int isInteger(char* string);

// Define the array and length as gloabal variable
int arr[MAX];
int n;

// Define the global count hash used in the count sort function
int countHash[MAX];
int maxElem,minElem;

// Define the parameters that need to be passed to the runner function
struct Param
{
	int split_num;
};

int getMaxElement(int arr[], int n);
int getMinElement(int arr[], int n);

int countSort(int minElem, int maxElem);
void *countSort_runner(void *arg);

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
			// Enter the Code Here
			maxElem = getMaxElement(arr,n);
			minElem = getMinElement(arr,n);

			// Perform count Sort
			countSort(minElem,maxElem);
			
			// Print the sorted array
			for(i=0;i<n;++i)
				printf("%d ",arr[i]);
			puts("");
		}
	}
	return 0;
}

// Define the count Sort Function
int countSort(int minElem, int maxElem)
{
	// We can perform the count sort in a multi-threaded fashion by performing the count sort parallely on different parititons
	int range = maxElem - minElem + 1;
	if(n < num_splits)
		num_splits = 2;

	// Declare the threads
	pthread_t tid[num_splits];
	int split_size = n/num_splits;

	// Define parameter structs to be passed to each runner
	struct Param param[num_splits];
	memset(countHash,0,sizeof(countHash));

	// Call the thread runners
	int j;
	for(j=0;j<num_splits;++j)
	{
		param[j].split_num = j;
		pthread_create(&tid[j],NULL,countSort_runner,(void *)&param[j]);
	}

	// Join the threads
	for(j=0;j<num_splits;++j)
		pthread_join(tid[j],NULL);

	// Cummulative count
	for(j=1;j<range;++j)
		countHash[j] += countHash[j-1]; 

	// Update the output array
	int output[n];
	for(j=n-1;j>=0;--j)
	{
		output[countHash[arr[j] - minElem] - 1] = arr[j];
		countHash[arr[j] - minElem]--;
	}

	// Update global array
	for(j=0;j<n;++j)
		arr[j] = output[j];
}

// Define the runner function for count Sort
void *countSort_runner(void *arg)
{
	// Get all arguments passed cast into appropriate types
	struct Param *param = (struct Param*)arg;
	int start = (n * param->split_num)/num_splits;
	int end = (n * (param->split_num + 1))/num_splits;

	// Loop and update countHash
	int i;
	for(i=start;i<end;++i)
		countHash[arr[i] - minElem]++;
}

// Define the utility function to get the maximum element from the array
int getMaxElement(int arr[], int n)
{
	int i, maxElem = arr[0];
	for(i=1;i<n;++i)
	{
		if(arr[i] > maxElem)
			maxElem = arr[i];
	}

	return maxElem;
}

int getMinElement(int arr[], int n)
{
	int i, minElem = arr[0];
	for(i=1;i<n;++i)
	{
		if(arr[i] < minElem)
			minElem = arr[i];
	}

	return minElem;
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