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
		// have to validate the string
		int sum  = 0;
		int n = atoi(argv[1]);
		pid_t pid = vfork();			// create child
		if(pid == 0)
		{	
			int i=0;
			for(;i<=n;++i)
				if(i%2 == 0)
					sum = sum + i;
			exit(0);
		}
		else							// compute odd series sum in parent.
		{
			int i=0;
			for(;i<=n;++i)
				if(i%2 == 1)
					sum = sum + i;
		}
		printf("Total Sum = %d\n",sum);
	}
	else
		printf("Too many arguments.\n");
	return 0;
}