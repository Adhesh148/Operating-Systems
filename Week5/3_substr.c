#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <inttypes.h>
#include <string.h>
#define MAX 1024

void substring(char str[], char substr[], int start, int len);

int main()
{
	// Define the file descriptor for the pipes
	int pipe_fd[2];

	// Create the pipes
	if(pipe(pipe_fd) == -1)
	{
		fprintf(stderr, "Pipe Failed\n");
		return 1;
	}

	// Fork to create process
	pid_t pid = fork();

	if(pid > 0)
	{
		// Within parent process
		printf("Enter the string.\n");
		char str[MAX];
		scanf("%[^\n]",str);

		// Close the read side of the pipe
		close(pipe_fd[0]);

		// send the string to the child process
		write(pipe_fd[1],str,strlen(str)+1);

	}
	else if(pid == 0)
	{
		// Within child process
		char str[MAX];

		// Close the write side of the pipe
		close(pipe_fd[1]);

		// read the string from the parent
		read(pipe_fd[0],str,MAX);

		// get substring arguments
		int start,len;
		printf("Enter the start position.\n");
		scanf("%d",&start);
		if(start >= strlen(str))
		{
			printf("Invalid start position.\n");
			exit(0);
		}
		printf("Enter the length of substring.\n");
		scanf("%d",&len);
	
		// generate substring
		char substr[MAX];
		substring(str,substr,start,len);
		printf("Substring is:\n%s\n",substr);

		exit(0);
	}
	else
	{
		fprintf(stderr, "Fork Failed\n");
		return 1;
	}

	wait(NULL);
	return 0;
}

void substring(char str[], char substr[], int start, int len)
{
	int i,pos=0;
	int str_len = strlen(str);
	for(i=start;i<start+len;++i)
	{
		if(i < str_len)
			substr[pos++] = str[i];
		else
			break;
	}

	substr[pos] = '\0';
}
