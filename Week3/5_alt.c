#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#define MAX 1000

void sortArray(int arr[],int n,int order);
int isInteger(char* string);

int main(int argc, char *argv[])
{

	if(argc < 2)
		printf("Insufficent arguments.\n");
	else 
	{
		int len = argc - 1;
		int arr[MAX],n=0,order=1;
		int i=1;
		for(;i<len;++i)
		{
		// Validation function. Warning if not integer.
		if(isInteger(argv[i]))
			arr[n++] = atoi(argv[i]);
		else
			printf("warning: '%s' is not an integer. will be ignored.\n",argv[i+1]);
		}

		// Warning if entire argument list is invalid.s
		if(n == 0 )
			printf("The input list is invalid. Please enter an integer array.\n");
		else
		{
			// Sort two ways - Desc in child and ascending in parent
			pid_t pid = vfork();
			wait(NULL);
			if(pid == 0)
			{
				// Within child - sort in descending order. order = 0;
				sortArray(arr,n,0);
				exit(0);
			}
			else
			{
				// since the array is already sorted by the child process, we can just print it in reverse to get the ascending output
				printf("The sorted (ascending) output is:\n");
				i=n-1;
				for(;i>=0;--i)
					printf("%d ",arr[i]);
				puts("");
			}
		}
	}
	return 0;
}

void sortArray(int arr[],int n,int order)
{
	int temp;													// For swap purposes

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

	// Print the sorted list.
	if(order == 1)
		printf("The sorted (ascending) output is:\n");
	else
		printf("The sorted (descending) output is:\n");
	i=0;
	for(;i<n;++i)
		printf("%d ",arr[i]);
	puts("");
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