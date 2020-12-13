#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <inttypes.h>

int main()
{
	unsigned long int count = 1;
	pid_t cpid = getpid();
	
	while(1)
	{
		if(getpid() == cpid)
		{
			++count;
			// printf("%ld\n",count);
			pid_t pid = fork();
			if(pid == 0 || pid == -1)
				break;
		}
	}

	if(getpid() == cpid)
		printf("Max Count: %ld\n",count);
	return 0;
}
