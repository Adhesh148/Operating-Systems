#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#define MAX 1000

int isInteger(char* string);
void merge(int arr[], int begin, int mid, int end);
void mergeSort(int arr[], int begin, int end);

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
			mergeSort(arr,0,n-1);
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

void mergeSort(int arr[], int begin, int end)
{
	if(begin < end)
	{
		int mid = begin + (end - begin)/2;

		// sort both halves - concurrently
		pid_t pid = vfork();
		if(pid == 0)
		{
			// Handle first half within child process
			mergeSort(arr,begin,mid);
			exit(0);
		}
		else if(pid > 0)
		{
			// Handle second half within parent process
			mergeSort(arr,mid+1,end);
		}
		wait(NULL);

		merge(arr,begin,mid,end);
	}
}

void merge(int arr[], int begin, int mid, int end)
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