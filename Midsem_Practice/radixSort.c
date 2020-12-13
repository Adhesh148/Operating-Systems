/******************************************
* AUTHOR : AdheshR *
* Problem Description: Multi-threaded version of Radix-Sort*
******************************************/
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
int countHash[10];

// Define the parameters that need to be passed to the runner function
struct Param
{
	int split_num;
	int digit;
};

int getMaxElement(int arr[], int n);
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
			int maxElem = getMaxElement(arr,n);
			int num_digits = floor(log(maxElem)/log(10) + 1);
			printf("%d\n",num_digits);

			// Loop through all digit places and perform countSort
			int i;
			for(i=0;i<num_digits;++i)
				countSort(i);
			
			// Print the sorted array
			for(i=0;i<n;++i)
				printf("%d ",arr[i]);
			puts("");
		}
	}
	return 0;
}

// Define the count Sort Function
int countSort(int digit)
{
	// We can perform the count sort in a multi-threaded fashion by performing the count sort parallely on different parititons
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
		param[j].digit = digit;
		pthread_create(&tid[j],NULL,countSort_runner,(void *)&param[j]);
	}

	// Join the threads
	for(j=0;j<num_splits;++j)
		pthread_join(tid[j],NULL);

	// Cummulative count
	for(j=0;j<10;++j)
		countHash[j] += countHash[j-1]; 

	// Update the output array
	int output[n];
	int exp = pow(10,digit);
	for(j=n-1;j>=0;--j)
	{
		output[countHash[(arr[j]/exp)%10] - 1] = arr[j];
		countHash[(arr[j]/exp)%10]--;
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
	int digit = param->digit;

	// Loop and update countHash
	int i;
	int exp = pow(10,digit);
	for(i=start;i<end;++i)
		countHash[(arr[i]/exp)%10]++;
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