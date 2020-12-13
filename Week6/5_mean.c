#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <inttypes.h>
#include <pthread.h>
#include <string.h>
#define MAX 100

// Define global parameters
long int sum_arr = 0;
int num_splits = 4, arr_len;

// allows the array to be passed by value (not by reference)
struct arrayWrapper
{
	int arr[MAX];
	int len;
	int split_no;
};

void *mean_runner(void *arg);
void *meanUtils_runner(void *arg);

// global declaration of arrayWrapper
struct arrayWrapper *A;

// struct to be used for merge sort
struct Position
{
	int start;
	int end;
};

void *mergeSort_thread(void *arg);
void merge(int begin, int mid, int end);

void *median_runner(void *arg);
void *mode_runner(void *arg);

int main(int argc, char *argv[])
{
	A = (struct arrayWrapper *)malloc(sizeof(struct arrayWrapper));

	if(argc < 2)
		printf("Insufficent arguments.\n");
	else 
	{
		int len = argc - 1;
		int i=1,n = 0;
		for(;i<=len;++i)
		{
		// Validation function. Warning if not integer.
		if(isInteger(argv[i]))
			A->arr[n++] = atoi(argv[i]);
		else
			printf("warning: '%s' is not an integer. will be ignored.\n",argv[i]);
		}
		
		// Update lengths
		A->len = n;
		arr_len = n;

		// Warning if entire argument list is invalid.s
		if(n == 0 )
			printf("The input list is invalid. Please enter an integer array.\n");
		else
		{
			// Create two threads - one for computing mean and other for sorting
			pthread_t tid[2];
			// Call the mean thread
			pthread_create(&tid[0],NULL,mean_runner,NULL);

			// Define the struct for the start and end positions to be passed
			struct Position *pos = (struct Position *)malloc(sizeof(struct Position));
			pos->start = 0;
			pos->end = n-1;
			// Call the sort thread
			pthread_create(&tid[1],NULL,mergeSort_thread,pos);

			// Join the threads
			pthread_join(tid[0],NULL);
			pthread_join(tid[1],NULL);

			// Call the median and mode function in two different threads
			pthread_create(&tid[0],NULL,median_runner,NULL);
			pthread_create(&tid[1],NULL,mode_runner,NULL);

			// Join both the threads
			float *median;
			pthread_join(tid[0],(void*)&median);

			int *mode;
			pthread_join(tid[1],(void*)&mode);

			// Display mean, median and mode
			printf("Mean = %lf\n",(double)sum_arr/(double)arr_len);
			printf("Median = %lf\n",*median);
			printf("Mode = %d\n",*mode);
		}
	}
	return 0;
}

// Main runner function to compute the Mean
void *mean_runner(void *arg)
{
	/* -- Sum the entire array in threaded fashion by simming each split in a different thread -- */
	if(arr_len < num_splits)
		num_splits = 2;

	// Declare the threads
	pthread_t tid[num_splits];

	int split_no = 0;
	int split_size = arr_len/num_splits;

	int i;
	struct arrayWrapper B[num_splits];
	for(i=0;i<num_splits;++i)
	{
		B[i] = *A;
		B[i].split_no = split_no;
		pthread_create(&tid[i],NULL,meanUtils_runner,(void *)&B[i]);
		++split_no;
	}

	for(i=0;i<num_splits;++i)
		pthread_join(tid[i],NULL);

	pthread_exit(NULL);
}

// sub thread runner to compute mean
void *meanUtils_runner(void *arg)
{
	// Find the start and end of array split
	struct arrayWrapper *A = (struct arrayWrapper*)arg;
	int start = (arr_len * A->split_no)/num_splits;
	int end = (arr_len * (A->split_no + 1))/num_splits;

	// Define a local sum
	long int local_sum = 0;

	// Loop to find sum
	int i;
	for(i=start;i<end;++i)
		local_sum = local_sum + A->arr[i];

	// Update global sum
	sum_arr = sum_arr + local_sum;

	// Exit the thread
	pthread_exit(NULL);
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

// Define the runner function for median function
void *median_runner(void *args)
{
	// Define a return pointer
	float *ret = malloc(sizeof(float));
	*ret = 0.0;

	if(arr_len % 2 == 1)
		*ret = A->arr[arr_len/2];
	else
		*ret = (A->arr[(arr_len-1)/2] + A->arr[arr_len/2])/2.0;

	pthread_exit(ret);
}

// Define the runnner function for the mode function
void *mode_runner(void *args)
{
	// Define the return pointer
	int *ret = malloc(sizeof(int));
	*ret = 0;

	int i=0, max_cnt = 0, curr = A->arr[0], cnt = 0;
	for(i=1;i<arr_len;++i)
	{
		if(A->arr[i]!= curr)
		{
			if(cnt > max_cnt)
			{
				max_cnt = cnt;
				*ret = curr;
			}
			curr = A->arr[i];
			cnt = 0;
		}
		else if(A->arr[i] == curr)
			++cnt;
	}

	if(max_cnt == 0)
		*ret = curr;

	pthread_exit(ret);
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
		left[i] = A->arr[begin + i];
	for(j=0;j<rsize;++j)
		right[j] = A->arr[mid+1+j];

	k = begin;
	i=0,j=0;

	while(i<lsize && j<rsize)
	{
		if(left[i] < right[j])
			A->arr[k] = left[i++];
		else
			A->arr[k] = right[j++];

		++k;
	}

	// Get the remaining elements from both the arrays into the merged array
	while(i<lsize)
		A->arr[k++] = left[i++];

	while(j<rsize)
		A->arr[k++] = right[j++];
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