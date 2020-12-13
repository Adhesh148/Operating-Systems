#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
	if(argc < 2)
		printf("Insufficient arguments\n");
	else if(argc == 2)
	{
		int sum = 0;
		int n = atoi(argv[1]);
		pid_t pid = fork();							// Create child process
		if(pid == 0)
		{	
			int i=0;
			printf("Even Series: ");
			for(;i<=n;++i)
				if(i%2 == 0)
					printf("%d ",i);
			puts("");
			exit(0);
		}
		else
		{
			int i=0;
			printf("Odd Series: ");
			for(;i<=n;++i)
				if(i%2 == 1)
					printf("%d ",i);
			puts("");
		}
	}
	else
		printf("Too many arguments.\n");
	return 0;
}