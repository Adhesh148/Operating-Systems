#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>
#include <inttypes.h>	

int main ()
{
	int pid[8];

	printf("Parent Process - 1. Process id = %jd\n",(intmax_t)getpid());

	pid[0] = fork();								// create process 2
	if(pid[0] == 0)
	{
		printf("Process - 2. Process id = %jd, Parent Process = %jd\n",(intmax_t)getpid(),(intmax_t)getppid());
		pid[2] = fork();  							// create process 4
		if(pid[2] == 0)
		{
			printf("Process - 4. Process id = %jd, Parent Process = %jd\n",(intmax_t)getpid(),(intmax_t)getppid());
			pid[6] = fork();						// create process 8
			if(pid[6] == 0)
				printf("Process - 8. Process id = %jd, Parent Process = %jd\n",(intmax_t)getpid(),(intmax_t)getppid());
		}
		else if(pid[2] > 0)
		{
			pid[3] = fork();						// create process 5
			if(pid[3] == 0)
			{
				printf("Process - 5. Process id = %jd, Parent Process = %jd\n",(intmax_t)getpid(),(intmax_t)getppid());
				pid[7] = fork();					// create process 9.
				if(pid[7] == 0)
					printf("Process - 9. Process id = %jd, Parent Process = %jd\n",(intmax_t)getpid(),(intmax_t)getppid());
			}
			else if(pid[3] > 0)
			{
				pid[4] = fork();     				// create process 6
				if(pid[4] == 0)
					printf("Process - 6. Process id = %jd, Parent Process = %jd\n",(intmax_t)getpid(),(intmax_t)getppid());
			}
		}
	}
	else if(pid[0] > 0)
	{
		pid[1] = fork();							// create process 3
		if(pid[1] == 0)
		{
			printf("Process - 3. Process id = %jd, Parent Process = %jd\n",(intmax_t)getpid(),(intmax_t)getppid());
			pid[5] = fork();						// create process 7
			if(pid[5] == 0)
				printf("Process - 7. Process id = %jd, Parent Process = %jd\n",(intmax_t)getpid(),(intmax_t)getppid());
		}
	}
}