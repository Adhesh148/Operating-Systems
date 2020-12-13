/******************************************
* AUTHOR : AdheshR *
* Problem Description: CAPS with pipes and dup*
******************************************/
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <inttypes.h>
#include <fcntl.h>
#define MAX 100

char *cmd1[] = {"ls","-al","/",0};
char *cmd2[] = {"tr","[a-z]","[A-Z]",0};

int main(int argcs, char *argv[])
{
	// printf("%s\n",argv[1]);
	//cmd1[1] = argv[1][0];
	cmd1[2] = argv[1];
	int pipe_fds[2];
	if(pipe(pipe_fds) == -1)
	{
		printf("Unable to create pipes.\n");
		exit(EXIT_FAILURE);
	}

	int pid;
	pid = fork();
	if(pid == 0)
	{
		/* --- Within child -- */

		// Close read part of pipe
		close(pipe_fds[0]);

		// Copy stdout fd to pipe write fd
		dup2(pipe_fds[1],1);

		// Execute ls command
		execvp(cmd1[0],cmd1);
	}
	else if(pid > 0)
	{
		/* --- Within parent --- */

		// Close write part of the fd
		close(pipe_fds[1]);

		// Copy pipe fd to stdin
		dup2(pipe_fds[0],0);

		// Exevute tr command
		execvp(cmd2[0],cmd2);
	}
	return 0;
}