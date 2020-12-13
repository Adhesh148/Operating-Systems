#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <inttypes.h>

int main()
{
	pid_t pid = fork();

	if(pid > 0)
		printf("This is the parent process.\n");
	else if(pid == 0)
	{
		sleep(10);
		printf("This is child. I am orphan.\n");
	}

	return 0;
}
