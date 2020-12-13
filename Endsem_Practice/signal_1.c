/******************************************
* AUTHOR : AdheshR *
* Problem Description: *
******************************************/
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <inttypes.h>
#include <fcntl.h>
#include <signal.h>
#define MAX 100

// define a global array
int len = 5;
int a[] = {1,2,3,4,5};

// declare the handler function
void handler(int signum)
{
	// If illegal access happens then return last entry
	printf("Captured: %d\n",a[len-1]);
}

int main()
{
	// Register the signal handler
	signal(SIGUSR1,handler);

	// Accpet the index from the user
	int indx;
	scanf("%d",&indx);

	if(indx > len)
		raise(SIGUSR1);
	else
		printf("%d\n",a[indx-1]);


	return 0;
}