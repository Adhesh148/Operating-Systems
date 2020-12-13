#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <inttypes.h>
#include <fcntl.h>
#define MAX 100

int main()
{
	int input_fds = open("./input.txt",O_RDONLY);
	int output_fds = open("./output.txt",O_WRONLY);

	// Copy STDIN to input_fds
	if(dup2(input_fds,STDIN_FILENO) < 0)
	{
		printf("Unable to create file descriptor\n");
		exit(EXIT_FAILURE);
	}

	// Copy STDOUT to output_fds
	if(dup2(output_fds,STDOUT_FILENO) < 0)
	{
		printf("Unable to create output file descriptor\n");
		exit(EXIT_FAILURE);
	}

	// Get input numbers from stdin (fd)
	int num1,num2;
	scanf("%d %d",&num1,&num2);

	// Compute sum
	int sum = num1 + num2;
	printf("%d + %d = %d \n",num1,num2,sum);
	return 0;
}