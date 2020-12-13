/******************************************
* AUTHOR : AdheshR *
* Problem Description: Prime Number generation upto n.*
******************************************/
#include <stdio.h>
#include <stdlib.h>
#define MAX 1000

void generatePrime(int n);
int isPrime(int num);

int main(int argc, char *argv[])
{
	// Define range
	int n = 13;

	// call function to generate prime upto n
	generatePrime(n);

	return 0;
}

// Define primeGenerate() function
void generatePrime(int n)
{
	int i=1;
	for(;i<=n;++i)
	{
		if(isPrime(i) == 1)
			printf("Prime: %d\n",i);
	}
}

// Utiltity function to check if the given number is prime or not
int isPrime(int num)
{
	if(num == 1)
		return 0;

	if(num == 2 || num == 3)
		return 1;

	int i = 2,flag = 0;
	for(i=2; i< num/2 +1;++i)
	{
		if(num%i == 0)
		{
			++flag;
			break;
		}
	}

	if(flag == 0)
		return 1;
	else 
		return 0;
}