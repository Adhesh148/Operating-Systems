/****************************************************
* AUTHOR : AdheshR*
* Question No.: Q2.i. * 
* Problem Statement: Design the sort for integers. *
*****************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX 100

void sortArray(int arr[],int n,int order);
int isInteger(char* string);

int main(int argc, char *argv[])
{
	int len = argc - 2;
	int order = atoi(argv[len+1]);
	// Ensure valid order is passed.
	if(order>1)
	{
		printf("Invalid Order Number used.\nDescending - 0, Asecending - 1.\n");
		return 0;
	}

	int arr[MAX],n=0;
	for(int i=0;i<len;++i)
	{
		// Validation function. Warning if not integer.
		if(isInteger(argv[i+1]))
			arr[n++] = atoi(argv[i+1]);
		else
			printf("warning: '%s' is not an integer. will be ignored.\n",argv[i+1]);
	}

	if(n == 0 )
		printf("The input list is invalid. Please enter an integer array.\n");
	else
		sortArray(arr,n,order);
	return 0;
}

void sortArray(int arr[],int n,int order)
{
	int temp;													// For swap purposes

	// Initiate Loops for simple Bubble Sort.
	for(int i=0;i<n;++i)
	{
		for(int j=0;j<n-i-1;++j)
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
	printf("The sorted output is:\n");
	for(int i=0;i<n;++i)
		printf("%d ",arr[i]);
	puts("");
}

// Validation Function
int isInteger(char* str)
{
	int character;
	for(int i=0;i<strlen(str);++i)
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