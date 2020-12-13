#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <inttypes.h>
#include <string.h>
#define MAX 1024

void concat(char str_1[], char str_2[], char concat_str[]);

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
		char str_1[MAX];
		scanf("%[^\n]",str_1);

		char str_2[MAX];

		// Close the write side of the first pipe & read side of second pipe
		close(pipe_fd_1[1]);
		close(pipe_fd_2[0]);

		// read string 2 from the child
		read(pipe_fd_1[0],str_2,MAX);

		// concat strings.
		char concat_str[MAX];
		concat(str_1,str_2,concat_str);

		// write the concatenated string to child
		write(pipe_fd_2[1],concat_str,strlen(concat_str)+1);

	}
	else if(pid == 0)
	{
		// Within child process
		char str[MAX];
		scanf("%[^\n]",str);

		// Close the read side of the first pipe & write side of second pipe
		close(pipe_fd_1[0]);
		close(pipe_fd_2[1]);

		// write the string to the parent
		write(pipe_fd_1[1],str, strlen(str)+1);

		// read the concatenated string from the parent
		char concat_str[MAX];
		read(pipe_fd_2[0],concat_str,MAX);

		printf("Concatenated string is:\n%s\n",concat_str);

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

void concat(char str_1[], char str_2[], char concat_str[])
{
	int len_1 = strlen(str_1),len_2 = strlen(str_2),i=0;
	for(i=0;i<len_1;++i)
		concat_str[i] = str_1[i];

	for(;i<len_1+len_2;++i)
		concat_str[i] = str_2[i-len_1];

	concat_str[i] = '\0';
}
