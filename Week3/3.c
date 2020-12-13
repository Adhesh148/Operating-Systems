// Amstrong Number Generation within a range
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
		int n = atoi(argv[1]);
		int i =0;
		for(;i<=n;++i)
		{
			int cube_i =0;
			pid_t pid = vfork();		// fork a child and wait for it to complete
			wait(NULL);
			if(pid == 0)				// child process - do digit extraction and cubing
			{
				int num = i,digit=0;
				while(num!=0)
				{
					digit = num%10;
					cube_i = cube_i + (digit * digit * digit);
					num = num/10;
				}
				exit(0);
			}
			else						// parent process - do check
			{
				if(cube_i == i)
					printf("%d is an Amstrong number\n",i);
			}
		}
	}
	else
		printf("Too many arguments.\n");
	return 0;
}