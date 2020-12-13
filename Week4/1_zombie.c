#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <inttypes.h>

int main()
{
	pid_t pid = fork();
	if(pid > 0)
	{
		printf("This is the parent process\n");
		sleep(10);
	}
	else if(pid == 0)
	{
		printf("Child process\n");
		exit(0);
	}
	return 0;
}
