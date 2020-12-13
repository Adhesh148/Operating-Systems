#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX 100
/**
Function Templates - 
**/
int isNumber(char* str,int *n_i);
void sortArray(void* arr[],int n,int order,char rep);
void swap(void **a,void **b,const int size);

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
		sortArray((void*)arrF,n_f,order,'f');
	else if(n_i>0)
		sortArray((void*)arrF,n_i,order,'i');

	if(n_c>0)
		sortArray((void*)arrC,n_c,order,'c');

	if(n_c == 0 && n_f == 0)
		printf("Invalid array. The elements of the array must be int/float/character.\n");
	
	return 0;
}

// Comparison part of Function Pointers.
// int desc(void *a, void *b)
// {
// 	int l = (int *)a; 
//     int r = (int *)b; 
// 	if(l<r)
// 	{
// 		printf("sad\n");
// 		return 1;
// 	}
// 	return 0;
// }

// int asc(void *a, void *b)
// {
// 	int l = ((int *)a); 
//     int r = ((int *)b); 
// 	if(l>r)
// 	{
// 		printf("yes\n");
// 		return 1;
// 	}
// 	// printf("yes\n");
// 	return 0;
// }

void sortArray(void* arr[],int n,int order,char rep)
{	
	// int (*compare[])(void*,void*) = {desc,asc};
	for(int i=0;i<n;++i)
	{
		for(int j=0;j<n-i-1;++j)
		{
			if((order == 1 && (((int *)arr)[j]>((int *)arr)[j+1])) || (order == 0 && (((int *)arr)[j]<((int *)arr)[j+1])))
			{
				
				// swap(&arr[j],&arr[j+1],sizeof(float));
				// printf("%f %f\n",((float *)arr)[j],((float *)arr)[j+1]);

				// printf("%f %f\n",((float *)arr)[j],((float *)arr)[j+1]);
				float temp = ((float *)arr)[j];
				float *swap = &temp;
				arr[j] = &arr[j+1];
				arr[j+1] = &swap;
				// *arr[j+1] = *temp;
			
			}
		}
	}

	// Print the sorted list.
	printf("The sorted output is:\n");
	if(rep == 'i')
		for(int i=0;i<n;++i)
			printf("%.0f ",((float *)arr)[i]);
	else if(rep == 'f')
		for(int i=0;i<n;++i)
			printf("%.2f ",((float *)arr)[i]);
	else
		for(int i=0;i<n;++i)
			printf("%c ",((char *)arr)[i]);
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

// Template Swap Function
void swap(void **a,void **b,const int size)
{
		printf("%f %f\n",*(float *)&a,*(float *)&b);
		void *temp = *a;
		*a = *b;
		*b = temp;
		printf("%f %f\n",*(float *)&a,*(float *)&b);
}