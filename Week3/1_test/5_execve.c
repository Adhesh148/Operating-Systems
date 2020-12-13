/******************************************
* AUTHOR : AdheshR *
* Problem Description: Test drive: execve*
******************************************/
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <inttypes.h>

char *const argv[] = {"ls",NULL};
char *const env[] = {"HOME=/usr/home","LOGNAME=home",(char*)0};

int main()
{
	pid_t pid;
	pid = fork();

	if(pid == 0)
	{
		// Execute ls command
		int ret = execve("/bin/ls",argv,env);
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
