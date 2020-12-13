#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <inttypes.h>

int main()
{
	int fd[3],count;
	printf("%d\n",fd[2]);
	if(pipe(fd)!=0)
		perror("Error");

	count = write(fd[2],"OS2018",7);
	printf("Count = %d\n",count);

	return 0;
}
