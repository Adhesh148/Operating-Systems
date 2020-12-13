/******************************************
* AUTHOR : AdheshR *
* Problem Description: 	Part - I: Multi-threaded version of Latin Matrix Generator
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
#include <time.h>
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

void orthogonalityCheck();

// Create a struct to pass the Positional arguments to the thread runner
struct Position
{
	int row;
	int col;
};

void *latin_runner(void *arg);
void *ortho_runner(void *arg);

int main()
{
	/* --  PART-I: Generate the Latin sqaure in multi-threaded manner 
		   Each row is generated in a different thread.     -- */

	// Input the Dimension of the Latin Square Matrix
	printf("\n************************* PART-I: Latin Square Generator. ***************************\n\n");
	printf("Enter the Dimension of the Latin Square Matrix to be generated.\n");
	scanf("%d",&N);

	// Create N threads
	pthread_t tid[N];

	// Create and init attr with default values
	pthread_attr_t attr;
	pthread_attr_init(&attr);

	// Call the thread runner for each thread
	int i;
	clock_t start, end;
	start = clock();
	for(i=0;i<N;++i)
	{
		// Define the argument to be passed to each thread runner
		int *row = malloc(sizeof(int));
		*row = i;
		pthread_create(&tid[i],&attr,latin_runner,row);
	}

	// Join all the N threads
	for(i=0;i<N;++i)
		pthread_join(tid[i],NULL);

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

void *latin_runner(void *arg)
{
	// Get the argument in appropriate type
	int row = *((int *)arg);

	// Fill in the row of the matrix
	int i = row;								// define start position in the matrix
	int fill = 1;							    // define the value to be filled in			
	while(fill <= N)
	{
		arr[row][i] = fill;
		i = (i + 1)%N;
		++fill;
	}

	pthread_exit(NULL);
}

// Define the function to perform the PART II of the question
void orthogonalityCheck()
{

	printf("\n********************* PART-II: Orthogonality Check of Two Latin Sqaure Matrices. ***********************\n\n");

	// Get the inputs from the user
	printf("Enter the order of Latin Squares.\n");
	scanf("%d",&order);

	// Loop variables
	int i,j;

	// Accept Square A
	printf("\nEnter the elements of Latin Square A.\n");
	for(i=0;i<order;++i)
		for(j=0;j<order;++j)
			scanf("%d",&A[i][j]);

	// Accept Square B
	printf("\nEnter the elements of Latin Square B.\n");
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

	/* -- Step - 1: Superimpose Matrix A and Matrix B to form Matrix C -- */
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

	/* --  Step -2: Check uniqueness of each element in the Matrix C in multi-threaded fashion. -- */

	// Create (order * order) threads for each entry in the C matrix
	pthread_t tid[order * order];
	int thread_no = 0;

	// Create and init attr with default values
	pthread_attr_t attr;
	pthread_attr_init(&attr);

	// Call the runner function
	struct Position pos[order * order];
	for(i=0;i<order;++i)
	{
		for(j=0;j<order;++j)
		{
			pos[thread_no].row = i;
			pos[thread_no].col = j;
			pthread_create(&tid[thread_no],&attr,ortho_runner,(void *)&pos[thread_no]);
			++thread_no;
		}
	}

	// Join all the threads
	thread_no =0;
	for(i=0;i<order;++i)
		for(j=0;j<order;++j)
			pthread_join(tid[thread_no++],NULL);

	/* --  Step -3: Evaluate the status of global variable orthogonalFlag and return status -- */
	if(orthogonalFlag == 0)
		return 1;
	return -1;
}

// Define the thread runner to check the orthogonality of the input matrix
void *ortho_runner(void *arg)
{
	struct Position *position = (struct Position*)arg;
	int row = position->row;
	int col = position->col;

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

	// Exit the thread
	pthread_exit(NULL);
}

