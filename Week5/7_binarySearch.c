#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <pthread.h>
#define MAX 1000

int isInteger(char* string);
void sortArray(int arr[],int start,int end,int order);
int binarySearch(int arr[], int start, int end, int x);

// define global array
int arr[MAX], arr_len;
// define size of each split - 4
int num_splits = 4;
// define global key
int x;
// global variable to maintain count
int count = 0;

void *runner(void *arg);

int main(int argc, char *argv[])
{	
	if(argc < 2)
		printf("Insufficent arguments.\n");
	else 
	{
		int len = argc-1;
		x = atoi(argv[len]);
		int n=0, i=1;
		for(;i<len;++i)
		{
			// Validation function. Warning if not integer.
			if(isInteger(argv[i]))
				arr[n++] = atoi(argv[i]);
			else
				printf("warning: '%s' is not an integer. will be ignored.\n",argv[i]);
		}
		// Assign length
		arr_len = n;

		// Warning if entire argument list is invalid.
		if(n == 0 )
			printf("The input list is invalid. Please enter an integer array.\n");
		else
		{
			/* -- Search for x in ? splits using Binary Search -- */
			if(arr_len < num_splits)
				num_splits = 2;

			// Declare the threads
			pthread_t tid[num_splits];

			int split_no = 0;
			int split_size = arr_len/num_splits;

			for(i=0;i<num_splits;++i)
			{
				int *arg = malloc(sizeof(*arg));
				*arg = split_no;
				pthread_create(&tid[i],NULL,runner,arg);
				++split_no;
			}

			for(i=0;i<num_splits;++i)
				pthread_join(tid[i],NULL);

			printf("%d is present %d times in the given array\n",x,count);
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

// Runner function for thread
void *runner(void *arg)
{
	int split_no = *((int *) arg);
	int start = (arr_len * split_no)/num_splits;
	int end = (arr_len * (split_no + 1))/num_splits;

	sortArray(arr,start,end,1);

	/* -- Perform Binary Search -- */
	int pos = binarySearch(arr,start,end - 1,x);
	int local_cnt = 0;
	if(pos!=-1)
	{
		++local_cnt;
		int j = pos+1;
		if(pos+1 < end)
		{
			while((arr[j]==x) && (j<end))
			{
				++local_cnt;
				j = j+1;
			}
		}
		j = pos-1;
		if(pos-1 >= start)
		{
			while((arr[j] == x) && (j>=start))
			{
				++local_cnt;
				j = j-1;
			}
		}
		printf("%d found in split-%d of the array %d times.\n",x,split_no+1,local_cnt);
	}
	else
		printf("%d not found in split-%d of the array.\n",x,split_no+1);
	
	count += local_cnt;
	pthread_exit(NULL);
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