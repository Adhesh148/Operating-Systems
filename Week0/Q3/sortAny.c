#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX 100
/**
This program handles array of ints,floats or characters (not strings) [All the elements in the array should be of a particular type, maybe with exception to floats and ints].
Since int and float only differ in their represenation of decimal points, a single array & function to handle both should be sufficient.
**/
int isNumber(char* str,int *n_i);
void sortArray(float arr[],int n,int order,char rep);
void sortArrayC(char arr[],int n,int order);

int main(int argc, char *argv[])
{
	int len = argc - 2;
	int order = atoi(argv[len+1]);
	if(order>1)
	{
		printf("Invalid Order Number used.\nDescending - 0, Asecending - 1.\n");
		return 0;
	}

	float arrF[MAX];
	char arrC[MAX];
	int n_f=0,n_c=0,n_i=0;

	for(int i=0;i<len;++i)
	{
		if(isNumber(argv[i+1],&n_i))
			arrF[n_f++] = atof(argv[i+1]);
		else if((strlen(argv[i+1]) == 1))			// Any one length not a number is a character.
			arrC[n_c++] = argv[i+1][0];
		else
			printf("warning: '%s' is not an integer/float/character. will be ignored.\n",argv[i+1]);
	}

	// Check if purely integer array
	if((n_f - n_i)>0)
		sortArray(arrF,n_f,order,'f');
	else if(n_i>0)
		sortArray(arrF,n_i,order,'i');

	if(n_c>0)
		sortArrayC(arrC,n_c,order);

	if(n_c == 0 && n_f == 0)
		printf("Invalid array. The elements of the array must be int/float/character.\n");
	
	return 0;
}

int desc(float a, float b)
{
	return a<b;
}

int asc(float a,float b)
{
	return a>b;
}

void sortArray(float arr[],int n,int order,char rep)
{
	int (*compare[])(float,float) = {desc,asc};
	float temp;
	for(int i=0;i<n;++i)
	{
		for(int j=0;j<n-i-1;++j)
		{
			if((*compare[order])(arr[j],arr[j+1]))
			{
				// Swap arr[j] and arr[j+1]
				temp = arr[j];
				arr[j] = arr[j+1];
				arr[j+1] = temp;			
			}
		}
	}

	// Print the sorted list.
	printf("The sorted output is:\n");
	if(rep == 'i')
	{
		for(int i=0;i<n;++i)
			printf("%.0f ",arr[i]);
		puts("");
	}
	else
	{
		for(int i=0;i<n;++i)
			printf("%.2f ",arr[i]);
		puts("");
	}
	
}

// Try to Overload. Doesnt seem possible...
void sortArrayC(char arr[],int n,int order)
{
	int (*compare[])(float,float) = {desc,asc};
	char temp;
	for(int i=0;i<n;++i)
	{
		for(int j=0;j<n-i-1;++j)
		{
			if((*compare[order])((int)arr[j],(int)arr[j+1]))
			{
				// Swap arr[j] and arr[j+1]
				temp = arr[j];
				arr[j] = arr[j+1];
				arr[j+1] = temp;			
			}
		}
	}

	// Print the sorted list.
	printf("The sorted output is:\n");
	for(int i=0;i<n;++i)
		printf("%c ",arr[i]);
	puts("");
}
	

// Validation Function
int isNumber(char* str,int *n_i)
{
	int character,flag=0;
	for(int i=0;i<strlen(str);++i)
	{
		character = str[i];
		// In case of '-'' symbol, continue if not last character in string.
		if(character == 45 && i<strlen(str)-1)
			continue;
		// In case of '.' symbol, continue only if it occurs once.
		if(character == 46 && flag == 0)
		{
			++flag;
			continue;
		}
		// Else get the decimal value (-48) and check condition.
		character -= 48;
		if(character < 0 || character > 9)
			return 0;
	}
	if(flag == 0)
		*n_i = *n_i+1;

	return 1;
}