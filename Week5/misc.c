#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <inttypes.h>
#include <string.h>
#include <pthread.h>
#define MAX 1024

int isArmstrong = -1;
void* runner(void *arg);

int main()
{
	// Get range.
	int n;
	printf("Enter the range.\n");
	scanf("%d",&n);

	printf("Armstrong numbers upto %d are:\n",n);

	// Loop to range.
	pthread_t tid[n];
	int i;
	for(i=0;i<=n;++i)
	{
		// Declare the argument to be passed to the thread
		int *arg = malloc(sizeof(*arg));
		*arg = i;

		// Create the thread
		pthread_create(&tid[i],NULL,&runner,arg);
	}

	int *ret;
	for(i=0;i<n;++i)
	{
		// Wait for the thread to exit and get return value 
		pthread_join(tid[i],(void*)&ret);
		if(*ret == 1)
			printf("%d\n",i);
	}
	
	return 0;
}

void* runner(void *arg)
{
	int num = *((int *) arg);
	free(arg);

	// Declare the return value
	int *ret = malloc(sizeof(int));
	*ret = 0;					// Default false condition

	// Perform the Armstrong Number Check
	long int cube_num =0, temp = num;
	int digit=0;
	while(temp!=0)
	{
		digit = temp%10;
		cube_num = cube_num + (digit * digit * digit);
		temp = temp/10;
	}

	if(cube_num == num)
		*ret = 1;

	pthread_exit(ret);
}