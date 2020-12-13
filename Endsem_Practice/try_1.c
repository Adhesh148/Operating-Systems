#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <inttypes.h>



int main()
{
	if(fork() && (!fork()))
	{
		if(fork() || fork())
			fork();
	}
	printf("OS\n");
	while(wait(NULL) > 0);
	return 0;
}
