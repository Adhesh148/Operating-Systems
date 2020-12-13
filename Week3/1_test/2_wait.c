/******************************************
* AUTHOR : AdheshR *
* Problem Description: Test drive: Wait()*
******************************************/
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <inttypes.h>

int main()
{
	pid_t child_pid;
	child_pid = fork();

	if(child_pid == 0)
	{
		// Do something
		printf("Inside child process.\n");
		exit(0);
	}
	else if(child_pid < 0)
		perror("Fork failure.\n");
	else
	{
		// Within parent
		int return_status;

		// Wait for the child
		waitpid(child_pid,&return_status,0);

		if(return_status == 0)
			printf("Child process terminated normally.\n");
		else
			printf("Child process terminated with an error.\n");

	}

	return 0;
}
