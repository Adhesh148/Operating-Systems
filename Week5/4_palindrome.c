#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <inttypes.h>
#include <string.h>
#define MAX 1024

int isPalindrome(char str[]);

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

		// Check if the string is a palindrome
		int status = isPalindrome(str);

		if(status == 1)
			printf("%s is a palindrome.\n",str);
		else
			printf("%s is NOT a palindrome.\n",str);
		
		exit(0);
	}
	else
	{
		fprintf(stderr, "Fork Failed\n");
		return 1;
	}
	return 0;
}

int isPalindrome(char str[])
{
	int len = strlen(str), i, flag =0;

	for(i=0; i<len;++i)
		if(str[i] != str[len-i-1])
			return 0;

	return 1;
}
