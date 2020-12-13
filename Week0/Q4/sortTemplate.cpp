/**********************************************************************
* AUTHOR : AdheshR*
* Question No.: Q4 * 
* Problem Statement: Design the sort function using Function Templates.
***********************************************************************/
#include <bits/stdc++.h>
using namespace std;
#define LL long long
#define MAX 100000
#define MOD 1000000007


int isNumber(char* str,int *n_i);
// Declare function templates for sort and swap.
template<class T> void sortArray(T arr[],int n,int order);
template<class T> void swapF(T &a,T &b);

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

	// Perform validation. Else warning.
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
		sortArray<float>(arrF,n_f,order);
	else if(n_i>0)
		sortArray<float>(arrF,n_i,order);

	if(n_c>0)
		sortArray<char>(arrC,n_c,order);

	if(n_c == 0 && n_f == 0)
		printf("Invalid array. The elements of the array must be int/float/character.\n");
	
	return 0;
}

template<class T> void sortArray(T arr[],int n,int order)
{
	for(int i=0;i<n;++i)
	{
		for(int j=0;j<n-i-1;++j)
		{
			if((order == 1 && (arr[j] > arr[j+1])) || (order == 0 && (arr[j] < arr[j+1])))
				swapF<T>(arr[j],arr[j+1]);
		}
	}

	printf("The sorted output is:\n");
	for(int i=0;i<n;++i)
		cout << arr[i]<<" ";
	cout << endl;
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

// Template Swap Function
template<class T> void swapF(T &a,T &b)
{
	T temp  = a;
	a = b;
	b = temp;
}