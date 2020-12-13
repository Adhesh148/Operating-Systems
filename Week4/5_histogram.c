#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <inttypes.h>
#include <string.h>
#include <time.h>
#define MAX 10000
int readFile(char src[],char buffer[]);

void updateCount(int charHash[],char string[],int begin,int end);
void countCharacters(char string[]);
void updateCountSequential(int charHash[],char string[],int end);

int main(int argc, char *argv[])
{
	if(argc <=1)
		printf("Insufficient arguments\n");
	else
	{	
		char buffer[MAX];
		int status = readFile(argv[1],buffer);
		countCharacters(buffer);
	}	
}

void countCharacters(char string[])
{
	// Maintain a hash table of size 256 corresponding to the 256 characters of the ASCII table
	int charHash[256] = {0};
	int len = strlen(string);

	updateCount(charHash,string,0,len-1);

	// print character count
	int i=0;
	for(;i<256;++i)
	{
		if(charHash[i]!=0)
		{
			if(i == 32)
				printf("space - %d\n",charHash[i]);
			else if(i == 10)
				printf("LF - %d\n",charHash[i]);
			else
				printf("%5c - %d\n",i,charHash[i]);
		}
	}
}

// A hash-based concurrent search algorithm. 
void updateCount(int charHash[],char string[],int begin,int end)
{
	if(begin==end)
		++charHash[(int)string[begin]];
	else if(begin<end)
	{
		int mid = begin + (end - begin)/2;
		pid_t pid = vfork();
		if(pid == 0)
		{
			updateCount(charHash,string,begin,mid);
			exit(0);
		}
		else if(pid > 0)
			updateCount(charHash,string,mid+1,end);
	}
}

int readFile(char src[],char buffer[])
{
	int i=0;

	// Open File
	FILE *fp = fopen(src,"r");

	while((buffer[i]=fgetc(fp))!= EOF)
		++i;

	buffer[i] = '\0';
}