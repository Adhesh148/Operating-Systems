/******************************************
* AUTHOR : AdheshR *
* Problem Description: Test drive: Wait()*
******************************************/
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <inttypes.h>
#define n 5

int main()
{
	pid_t child_pid,wpid;
	int status = 0;

	// Parent code (before child processes starts)
	printf("Parent Process BEFORE children !!\n");

	int id = 0;
	for(id = 0; id < n; ++ id)
	{
		if((child_pid == fork()) == 0)
		{
			// Child code
			printf("Child - %d\n",id);
			exit(0);
		}
	}

	// Parent waits for all children
	while((wpid = wait(&status)) != -1);

	// Parent code (after all children)
	printf("Parent AFTER children !!\n");

	return 0;
}
