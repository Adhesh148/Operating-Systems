/*******************************************************************
* AUTHOR : AdheshR*
* Question No: Q3.
* Problem Statement: Design the sort using function overloading. *
*******************************************************************/

#include <bits/stdc++.h>
using namespace std;
#define LL long long
#define MAX 100000
#define MOD 1000000007
#define dd double

int isNumber(char* str,int *n_i);
void sortArray(float arr[],int n,int order,char rep);
void sortArray(char arr[],int n,int order);

int main(int argc, char *argv[])
{
	int len = argc - 2;
	int order = atoi(argv[len+1]);
	// Validate order.
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
		// Ensure number (int or float) or character. Else warning.
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
		sortArray(arrC,n_c,order);

	if(n_c == 0 && n_f == 0)
		printf("Invalid array. The elements of the array must be int/float/character.\n");
	
	return 0;
}

// Helper function for the compare funciton pointer.
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
	// Declare the function pointer.
	int (*compare[])(float,float) = {desc,asc};
	float temp;											// for swap purposes.

	// Initialiaze loop for the Bubble Sort.
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

// Overload the function - for character array.
void sortArray(char arr[],int n,int order)
{
	int (*compare[])(float,float) = {desc,asc};
	char temp;

	// Bubble Sort.
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
	

// Validation Function - checks for float or int.
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