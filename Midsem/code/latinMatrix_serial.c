/******************************************
* AUTHOR : AdheshR *
* Problem Description: 	Part - I:  Serial version of Latin Matrix Generator
						Part - II: Given two input latin squares - check their orthogonality*
******************************************/
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <pthread.h>
#include <math.h>
#define MAX 1000

// Define the array and length as gloabal variable
int arr[MAX][MAX];
int N;

// Define global variables for PART II - Checking orthogonality
int A[MAX][MAX];
int B[MAX][MAX];
int C[MAX][MAX];
int order;
int orthogonalFlag = 0;

void latinUtils(int row);
void orthoUtils(int row, int col);
void orthogonalityCheck();

int main()
{
	/* --  PART-I: Generate the Latin sqaure in serial -- */

	// Input the Dimension of the Latin Square Matrix
	printf("\n************************* PART-I: Latin Square Generator. ***************************\n\n");
	printf("Enter the Dimension of the Latin Square Matrix to be generated.\n");
	scanf("%d",&N);

	// Call the utils for each row
	int i;
	clock_t start, end;
	for(i=0;i<N;++i)
		latinUtils(i);
	end = clock();
	printf("\nTime taken to generate Matrix = %lf\n",(double)(end - start));

	// Print the matrix
	int j;
	printf("\nA Latin Square of order %d is:\n",N);
	for(i=0;i<N;++i)
	{
		for(j=0;j<N;++j)
			printf("%d ",arr[i][j]);
		puts("");
	}

	/* -- PART II - Checking Orthogonality of two Input Latin Square matrices -- */
	start = clock();
	orthogonalityCheck();
	end = clock();
	printf("\nTime taken to check orthogonality = %lf\n",(double)(end - start));

	return 0;
}

void latinUtils(int row)
{
	// Fill in the row of the matrix
	int i = row;								// define start position in the matrix
	int fill = 1;							    // define the value to be filled in			
	while(fill <= N)
	{
		arr[row][i] = fill;
		i = (i + 1)%N;
		++fill;
	}
}

// Define the function to perform the PART II of the question
void orthogonalityCheck()
{

	printf("\n******************** PART-II: Orthogonality Check of Two Latin Sqaure Matrices. ***********************\n\n");

	// Get the inputs from the user
	printf("Enter the order of Latin Squares.\n");
	scanf("%d",&order);

	// Loop variables
	int i,j;

	// Accept Square A
	printf("\nEnter the elements of Latin Sqaure A.\n");
	for(i=0;i<order;++i)
		for(j=0;j<order;++j)
			scanf("%d",&A[i][j]);

	// Accept Square B
	printf("\nEnter the elements of Latin Sqaure B.\n");
	for(i=0;i<order;++i)
		for(j=0;j<order;++j)
			scanf("%d",&B[i][j]);

	int status = isOrthogonal(order);

	if(status == -1)
		printf("\nGiven Latin Square Matrices are NOT Mutually Orthogonal.\n");
	else
		printf("\nGiven Latin Square Matrices are Mutually Orthogonal.\n");
}

int isOrthogonal(int order)
{
	// Let us first eliminate the trivial cases

	// Based on Eulers conjecture
	if(order == 6)
		return -1;
		
	// Now for the remaining orders - let us check the the orthogonality

	/* -- Step - 1: Superimpose Matrix A and Matrix B to form Matrix C  -- */
	int i,j;
	int num_digits_B = 0;
	int exp = 0;
	for(i=0;i<order;++i)
	{
		for(j=0;j<order;++j)
		{
			num_digits_B = floor(log10(B[i][j]) + 1);
			exp = pow(10,num_digits_B);
			C[i][j] = A[i][j] * exp + B[i][j];
		}
	}

	/* --  Step -2: Check uniqueness of each element in the Matrix C. -- */
	for(i=0;i<order;++i)
		for(j=0;j<order;++j)
			orthoUtils(i,j);

	/* --  Step -3: Evaluate the status of global variable orthogonalFlag and return status -- */
	if(orthogonalFlag == 0)
		return 1;
	return -1;
}

// Define the utility function to check uniqueness of the 
void orthoUtils(int row, int col)
{
	int i,j;
	int flag = 0;
	for(i=row;i<order;++i)
	{
		for(j=col;j<order;++j)
		{
			// Skip for that position
			if(i == row && j == col)
				continue;

			if(C[i][j] == C[row][col])
			{
				++flag;
				break;
			}
		}

		if(flag!=0)
			break;
	}

	if(flag != 0)
		++orthogonalFlag;
}

