#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <inttypes.h>
#include <pthread.h>
#include <string.h>
#define MAX 1000

// structure to pass positional values for two strings
struct Param
{
	int m,n;
};

// Global declaration of the two strings
char s1[MAX], s2[MAX];

void *lcs_thread(void *arg);
int max(int a, int b);

int main()
{
	// Accept both strings as input
	printf("Enter the first string\n");
	scanf("%s",s1);

	printf("Enter the second string\n");
	scanf("%s",s2);

	// Instantiate a struct
	struct Param *param = (struct Param*)malloc(sizeof(struct Param));
	param->m = strlen(s1);
	param->n = strlen(s2);

	// Create a main thread (initial thread)
	pthread_t tid;
	pthread_create(&tid,NULL,lcs_thread,param);

	// Define return value from thread
	int *max_len;
	pthread_join(tid,(void*)&max_len);

	printf("Length of LCS is %d\n",*max_len);

	return 0;
}

// Define the LCS Thread
void *lcs_thread(void *arg)
{
	int m = ((struct Param*)arg)->m;
	int n = ((struct Param*)arg)->n;

	// Define return variable
	int *ret = malloc(sizeof(int));
	*ret = 0;

	// Trivial condition
	if(m == 0 || n == 0)
		pthread_exit(ret);

	if(s1[m-1] == s2[n-1])
	{
		// Define a new struct copied from the arg
		struct Param arg_1 = *((struct Param*)arg);
		--arg_1.m;
		--arg_1.n;

		// Recursively call the thread again
		pthread_t tid;
		pthread_create(&tid,NULL,lcs_thread,(void *)&arg_1);

		// Get return value from thread
		int *ret_1;
		pthread_join(tid,(void*)&ret_1);
		*ret = *ret_1 + 1;
	}
	else
	{
		// Define new structs copied from the arg
		struct Param arg_1 = *((struct Param*)arg);
		--arg_1.n;

		struct Param arg_2 = *((struct Param*)arg);
		--arg_2.m;

		// Recursively call the thread again
		pthread_t tid[2];
		pthread_create(&tid[0],NULL,lcs_thread,(void *)&arg_1);
		pthread_create(&tid[1],NULL,lcs_thread,(void *)&arg_2);

		// Get return value from both threads
		int *ret_1, *ret_2;
		pthread_join(tid[0],(void*)&ret_1);
		pthread_join(tid[1],(void*)&ret_2);

		*ret = max(*ret_1,*ret_2);
	}
	pthread_exit(ret);
}

int max(int a, int b)
{
	if(a > b)
		return a;
	return b;
}