#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

long int fibonacci(int  n);

int main(int argc, char *argv[])
{
	if(argc < 2)
		printf("Insufficient arguments\n");
	else if(argc == 2)
	{
		int n = atoi(argv[1]);
		long int fib = fibonacci(n);
		printf("%dth term of Fibonacci Series is: %ld\n",n,fib);
	}
	else
		printf("Too many arguments.\n");
	return 0;
}

long int fibonacci(int  n)
{
	if(n<=2)
		return 1;
	else
	{
		pid_t pid = vfork();
		long int fib_0,fib_1;
		if(pid == 0)
		{
			fib_0 = fibonacci(n-2);
			exit(0);
		}
		else if(pid > 0)
		{
			fib_1 = fibonacci(n-1);
		}
		wait(NULL);
		return fib_0 + fib_1;
	}
}

