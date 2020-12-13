/******************************************
* AUTHOR : AdheshR *
* Problem Description: Find all the file names that are palindromes*
******************************************/
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <inttypes.h>
#include <string.h>
#define MAX 1024

char *const argv[] = {"ls",NULL};
char out_buffer[1024];

void checkPalindrome(char token[MAX]);

int main()
{
	// Define and create pipes
	int pipe_fd[2];

	if(pipe(pipe_fd) == -1)
	{
		perror("Error in creating pipes");
		exit(1);
	}

	pid_t pid = fork();

	if(pid > 0)
	{
		// Within the parent

		// Close the write end
		close(pipe_fd[1]);

		// Read from the read end of the pipe
		read(pipe_fd[0],out_buffer,sizeof(out_buffer));

		// Split the outputs and check if they are palindromes
		char *token;
		token = strtok(out_buffer,"\n");
		char file_name[MAX];

		while(token != NULL)
		{
			printf("Token  = %s\n",token);
			checkPalindrome(token);
			token = strtok(NULL,"\n");
		}

	}
	else if(pid == 0)
	{
		// Within the child

		// Close the read end of the pipe
		close(pipe_fd[0]);

		// Dup the write end
		dup2(pipe_fd[1],1);

		// Execute ls command 
		execvp(argv[0],argv);
	}
	else
	{
		perror("Failed to fork");
		exit(1);
	}

	return 0;
}

void checkPalindrome(char file_name[MAX])
{
	printf("%s\n",file_name);

}
