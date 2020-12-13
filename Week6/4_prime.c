#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <inttypes.h>
#include <string.h>
#include <pthread.h>

int isInteger(char* str);
void *runner(void *arg);
int isPrime(int n);

int main(int argc, char *argv[])
{
	// Validate the input
	if(argc <= 1)
	{
		printf("Insufficient arguments.\n");
		return 0;
	}

	// Get the input n.
	int n;
	if(isInteger(argv[1]))
		n = atoi(argv[1]);
	else
	{
		printf("Invalid input.\n");
		return 0;
	}

	// Declare n threads
	pthread_t tid[n];

	// Loop to n.
	int i=0;
	for(;i<=n;++i)
	{
		// create a thread that checks the primality of i
		int *arg = malloc(sizeof(*arg));
		*arg = i;
		pthread_create(&tid[i],NULL,runner,arg);
	}

	// Join all threads
	int *primality;
	i=0;
	for(;i<=n;++i)
	{
		// Get primality status and join thread
		pthread_join(tid[i],(void*)&primality);
		if(*primality == 1)
			printf("%d\n",i);
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

void *runner(void *arg)
{
	// Cast arg to integer
	int n = *((int *) arg);

	// Declare the return value 
	int *ret = malloc(sizeof(int));
	*ret = 0;

	// Check primality of n
	*ret = isPrime(n);

	// Exit the thread and return primality status
	pthread_exit(ret);
}

// isPrime Utility Function
int isPrime(int n) 
{ 
    // Corner cases 
    if (n <= 1)
     return 0; 
    if (n <= 3) 
    	return 1; 
  	
    if (n%2 == 0 || n%3 == 0)
    	return 0; 
  	
  	int i;
    for (i=5; i*i<=n; i=i+6) 
        if (n%i == 0 || n%(i+2) == 0) 
           return 0; 
  
    return 1; 
}