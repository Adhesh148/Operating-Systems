/******************************************
* AUTHOR : AdheshR *
* Problem Description: Understand that process created using fork creates different address space	*
******************************************/
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <inttypes.h>

int b = 50;
int main()
{
	int pid;
	int a = 5;

	pid = fork();
	if(pid > 0)
	{
		a++;
		b++;
		printf("%p %p\n",&a,&b );
		printf("Values of a and b are %d and %d\n",a,b);
	}
	if(pid == 0)
	{
		a = a+2;
		b = b+2;
		printf("%p %p\n",&a,&b );
		printf("Values of a and b are %d and %d\n",a,b);
	}
	return 0;
}
