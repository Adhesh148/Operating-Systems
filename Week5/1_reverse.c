#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <inttypes.h>
#include <string.h>
#define MAX 1024

void reverse(char str[], char str_rev[]);

int main()
{
	// Define the file descriptor for the pipes
	int pipe_fd_1[2];
	int pipe_fd_2[2];

	// Create the pipes
	if(pipe(pipe_fd_1) == -1)
	{
		fprintf(stderr, "Pipe Failed\n");
		return 1;
	}

	if(pipe(pipe_fd_2) == -1)
	{
		fprintf(stderr, "Pipe 2 Failed\n");
	}

	// Fork to create process
	pid_t pid = fork();

	if(pid > 0)
	{
		// Within parent process
		char str[MAX];
		scanf("%[^\n]",str);

		// Close the read side of the first pipe & write side of second pipe
		close(pipe_fd_1[0]);
		close(pipe_fd_2[1]);

		// send the string to the child process
		write(pipe_fd_1[1],str,strlen(str)+1);

		// read reversed string from parent
		char str_rev[MAX];
		read(pipe_fd_2[0],str_rev,strlen(str)+1);

		// print the reversed string
		printf("Reversed string is:\n%s\n",str_rev);
	}
	else if(pid == 0)
	{
		// Within child process
		char str[MAX];

		// Close the write side of the first pipe & read side of second pipe
		close(pipe_fd_1[1]);
		close(pipe_fd_2[0]);

		// read the string from the parent
		read(pipe_fd_1[0],str,MAX);

		// reverse string and write to parent
		char str_rev[MAX];
		reverse(str,str_rev);
		write(pipe_fd_2[1],str_rev,strlen(str)+1);

		exit(0);
	}
	else
	{
		fprintf(stderr, "Fork Failed\n");
		return 1;
	}

	return 0;
}

void reverse(char str[], char str_rev[])
{
	int len = strlen(str),i;
	for(i=len-1;i>=0;--i)
		str_rev[len-1-i] = str[i];
	str_rev[len] = '\0';
}
