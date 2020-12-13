#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#define MAX 1000

void sortArray(int arr[],int start,int end,int order);
int isInteger(char* string);
int binarySearch(int arr[], int start, int end, int x);

int main(int argc, char *argv[])
{

	if(argc < 2)
		printf("Insufficent arguments.\n");
	else 
	{
		int len = argc - 2,x;
		if(isInteger(argv[argc -1]))
			x = atoi(argv[argc-1]);
		else
		{
			printf("Invalid search key provided.\n");
			return 0;
		}
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
			pid_t pid = vfork();
			
			if(pid == 0)
			{
				sortArray(arr,0,n/2,1);
				int pos = binarySearch(arr,0,(n/2) - 1,x);
				if(pos!=-1)
				{
					int count = 1;
					int j = pos+1;
					if(pos+1 < n/2)
					{
						while((arr[j]==x) && (j<n/2))
						{
							count = count +1;
							j = j+1;
						}
					}
					j = pos-1;
					if(pos-1 >= 0)
					{
						while((arr[j] == x) && (j>=0))
						{
							count = count + 1;
							j = j-1;
						}
					}
					printf("%d found in first half of the array %d times.\n",x,count);
				}
				exit(0);
			}
			else
			{
				sortArray(arr,n/2,n,1);
				int pos = binarySearch(arr,n/2,n-1,x);
				if(pos!=-1)
				{
					int count = 1;
					int j = pos+1;
					if(pos+1 < n)
					{
						while((arr[j]==x) && (j<n))
						{
							++count;
							++j;
						}
					}
					j = pos-1;
					if(pos-1 >= n/2)
					{
						while((arr[j] == x) && (j>=n/2))
						{
							++count;
							--j;
						}
					}
					printf("%d found in second half of the array %d times.\n",x,count);
				}
			}
			wait(NULL);
		}
	}
	return 0;
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

	i=start;
	for(;i<end;++i)
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

int binarySearch(int arr[], int start, int end, int x) 
{ 
    if (end >= start) 
    { 
        int mid = start + (end - start) / 2; 

        if (arr[mid] == x) 
            return mid; 

        if (arr[mid] > x) 
            return binarySearch(arr, start, mid - 1, x); 
  
        return binarySearch(arr, mid + 1, end, x); 
    } 

    return -1; 
} 