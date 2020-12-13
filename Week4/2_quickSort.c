#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <inttypes.h>
#include <sys/wait.h>
#include <string.h>
#define MAX 1000

int isInteger(char* string);
void quickSort(int arr[], int begin, int end);
int partition(int arr[],int begin, int end);

int main(int argc, char *argv[])
{

	if(argc < 2)
		printf("Insufficent arguments.\n");
	else 
	{
		int len = argc - 1;
		int arr[MAX],n=0,order=1;
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
			// Enter Code Here
			quickSort(arr,0,n-1);
			int i=0;
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

// Quick Sort Function
void quickSort(int arr[], int begin, int end)
{
	if(begin < end)
	{
		int p = partition(arr,begin,end);

		// sort both halves - concurrently
		pid_t pid = vfork();
		if(pid == 0)
		{
			// Handle first half within child process
			quickSort(arr,begin,p-1);
			exit(0);
		}
		else if(pid > 0)
		{
			// Handle second half within parent process
			quickSort(arr,p+1,end);
		}
		wait(NULL);
	}
}

// QuickSort - Partition Function
int partition(int arr[],int begin, int end)
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