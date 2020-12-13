/******************************************
* AUTHOR : AdheshR *
* Problem Description: Test drive: execl*
******************************************/
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <inttypes.h>

int main()
{
	pid_t pid;
	pid = fork();

	if(pid == 0)
	{
		// Execute ls command
		int ret = execl("/bin/ls","ls",NULL);
		if (ret < 0)
			perror("Couldnt execute");
	}
	else
	{
		wait(NULL);
		printf("Parent has control\n");
		printf("Parent has waited for child to complete\n");
	}
	return 0;
}
