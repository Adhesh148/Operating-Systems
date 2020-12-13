#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>
#include <inttypes.h>

void *fib_thread(void *arg);

int main()
{
	// Accept input n
	int n;
	printf("Enter n\n");
	scanf("%d",&n);

	// Generate nth fibonacci number
	int *arg = malloc(sizeof(*arg));
	*arg = n;

	pthread_t tid;
	pthread_create(&tid,NULL,fib_thread,arg);
	
	// Join back the thread
	int *fib_n;
	pthread_join(tid,(void*)&fib_n);

	printf("%dth Fibonacci number is: %d\n",n,*fib_n);

	return 0;	
}

void *fib_thread(void *arg)
{
	// cast to int
	int n = *((int *) arg);

	// Declare the return variable
	int *ret = malloc(sizeof(int));
	*ret = 0;

	if(n <= 2)
		*ret = 1;
	else
	{
		int *arg_1 = malloc(sizeof(*arg));
		int *arg_2 = malloc(sizeof(*arg));
		
		// Declare two pthreads
		pthread_t tid[2];

		// Declare two return integer pointers
		int *ret_1, *ret_2;

		// Create two threads for each recursion tree
		*arg_1 = n-1;
		pthread_create(&tid[0],NULL,fib_thread,arg_1);

		*arg_2 = n-2;
		pthread_create(&tid[1],NULL,fib_thread,arg_2);

		// Join both threads back
		pthread_join(tid[0],(void*)&ret_1);
		pthread_join(tid[1],(void*)&ret_2);

		// return the sum
		*ret = *ret_1 + *ret_2;
	}
	pthread_exit(ret);
}
