// Fibonacci and Prime Series Generation
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#define MAX 1000

int isPrime(long int n);

int main(int argc, char *argv[])
{
	if(argc < 2)
		printf("Insufficient arguments\n");
	else if(argc == 2)
	{
		// have to validate the string
		int sum = 0;
		int n = atoi(argv[1]);
		pid_t pid = fork();
		if(pid == 0)
		{
			// Do the Prime series generation
			int i=1;
			printf("Prime Series: ");
			for(;i<=n;++i)
				if(isPrime(i))
					printf("%d ",i);
			puts("");
			exit(0);
		}
		else
		{
			// Do the fibonacci Series generation upto nth term
			int fib[MAX] = {0};
			fib[0] = 0;
			fib[1] = 1;
			int i=1;
			while(fib[i] <= n)
			{
				fib[i+1] = fib[i] + fib[i-1];
				i = i+1;
			}
			int j=0;
			printf("Fibonacci Series: ");
			for(;j<i;++j)
				printf("%d ",fib[j]);
			puts("");
		}
	}
	else
		printf("Too many arguments.\n");
	return 0;
}

int isPrime(long int n) 
{ 
    // Corner cases 
    if (n <= 1)  return 0; 
    if (n <= 3)  return 1; 
  
    // This is checked so that we can skip  
    // middle five numbers in below loop 
    if (n%2 == 0 || n%3 == 0) return 0; 
  	
  	int i = 5;
    for (; i*i<=n; i=i+6) 
        if (n%i == 0 || n%(i+2) == 0) 
           return 0; 
  
    return 1; 
} 