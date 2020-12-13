#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

long int fibonacciNumber(int n);
long int fibonacciThread(int n);

int main(int argc, char ** argv)
{
    double time_spent = 0.0;

    if(argc == 2)
    {
        int n = atoi(argv[1]);
        clock_t begin = clock();
        long int fib_n = fibonacciThread(n);
        clock_t end = clock();
        printf("%ld\n",fib_n);

        time_spent += (double)(end - begin) / CLOCKS_PER_SEC;
        printf("Execution time = %f\n",time_spent);
    }
    else
        printf("Too many arguments\n");

    return 0;
}

// Simple recursive function
long int fibonacciNumber(int n)
{
    if(n <= 1)
        return 1;
    else
        return fibonacciNumber(n-1) + fibonacciNumber(n-2);
}

// Parallelised function
long int fibonacciThread(int n)
{
    if(n <= 1)
        return 1;
    else
    {
        int pid = vfork();
        long int fib_0=0,fib_1=0;
        
        if(pid == 0)
        {
            fib_0 = fibonacciThread(n-1);
            exit(0);
        }
        else
            fib_1 = fibonacciThread(n-2);
        wait(NULL);
        printf("%ld %ld\n",fib_0,fib_1);
        return fib_0 + fib_1;

        
    }
}