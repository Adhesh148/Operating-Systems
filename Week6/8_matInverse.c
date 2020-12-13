#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <inttypes.h>
#include <pthread.h>

#define MAX 100

// declare an array structure
struct arrayWrapper
{
	double arr[MAX][MAX];
	int N;
	int n;
};

// Declare global variables
struct arrayWrapper *A;
struct arrayWrapper *adj;
int N;

void *det_runner(void *arg);
void *adjoint_runner(void *arg);

int main()
{
	A = (struct arrayWrapper *)malloc(sizeof(struct arrayWrapper));
	adj = (struct arrayWrapper *)malloc(sizeof(struct arrayWrapper));

	// Accept input
	printf("Enter the dimension (N)\n");
	scanf("%d",&N);
	A->N = N;
	A->n = N;

	// Accept the matrix
	printf("Enter the %d * %d matrix.\n",A->N,A->N);
	int i,j;
	for(i=0;i<N;++i)
	{
		for(j=0;j<N;++j)
		{
			scanf("%lf",&(A->arr[i][j]));
		}
	}

	// Create two threads to compute the adjoint and determinant parallely
	pthread_t tid[2];

	// Call the thread - to compute determinant
	struct arrayWrapper B  = *A;
	pthread_create(&tid[0],NULL,det_runner,(void *)&B);

	// Call the thread to compute the adjoint
	adj->N = N;
	pthread_create(&tid[1],NULL,adjoint_runner,NULL);

	// Join both the threads
	int *det;
	pthread_join(tid[0],(void*)&det);
	printf("Determinant = %d\n",*det);

	// Join adj thread
	pthread_join(tid[1],NULL);

	// Print the inverse matrix
	printf("Inverse of Matrix is:\n");
	for(i=0;i<N;++i)
	{
		for(j=0;j<N;++j)
			printf("%9lf ",adj->arr[i][j]/(float)*det);
		puts("");
	}

	return 0;
}

// Define the runner thread to compute the determinant
void *det_runner(void *arg)
{
	// Cast the arg
	struct arrayWrapper *B = (struct arrayWrapper*)arg;
	int n = B->n;

	// Define the return value
	int *det = malloc(sizeof(int));
	*det = 0;

	// Base case
	if(n == 1)
	{
		*det = B->arr[0][0];
		pthread_exit(det);
	}

	// Define variables
	int sign = 1;
	int i,j,k,row,col;

	// Define temp struct to store the cofactors in each loop
	struct arrayWrapper temp[n];

	for(i=0;i<n;++i)
	{
		// Compute Cofactor in place - passing to a function causes prolem
		j=0,k=0;
		for(row=0;row<n;++row)
		{
			for(col=0;col<n;++col)
			{
				if(row!=0 && col!=i)
				{
					temp[i].arr[j][k++] = B->arr[row][col];

					if(k == n-1)
					{
						k = 0;
						++j;
					}
				}
			}
		}
		temp[i].n = n-1;

		// Create a new thread to calc the determinant of the cofactor
		pthread_t tid;
		pthread_create(&tid,NULL,det_runner,(void *)&temp[i]);

		// Join the thread and get return value
		int *local_det;
		pthread_join(tid,(void*)&local_det);
		
		// Compute the determinant
		*det = *det + sign * B->arr[0][i] * (*local_det);

		// Flip the sign
		sign = -1*sign;
	}

	// Exit and return the determinant.
	pthread_exit(det);
}

// Define the adjoint runner thread
void *adjoint_runner(void *arg)
{
	if(N == 1)
	{
		adj->arr[0][0] = 1;
		pthread_exit(NULL);
	}

	// Define cofactor array
	struct arrayWrapper cofactor[N][N];
	
	int sign = 1;

	// Loop
	int i,j,row,col,p,q;
	for(i=0;i<N;++i)
	{
		for(j=0;j<N;++j)
		{
			// Compute Co-factor inplace
			cofactor[i][j].N = N-1;
			cofactor[i][j].n = N-1;
			p=0,q=0;
			for(row=0;row<N;++row)
			{
				for(col=0;col<N;++col)
				{
					if(row!=i && col!=j)
					{
						cofactor[i][j].arr[p][q++] = A->arr[row][col];

						if(q == N-1)
						{
							q = 0;
							++p;
						}
					}
				}
			}

			// determine sign
			if((i+j)%2 == 0)
				sign = 1;
			else
				sign = -1;

			// Get the determinant of cofactor matrix by calling the determinant thread
			int *det;
			pthread_t tid;
			pthread_create(&tid,NULL,det_runner,(void *)&cofactor[i][j]);			
			pthread_join(tid,(void*)&det);

			adj->arr[j][i]  = sign * (*det);
		}
	}

	pthread_exit(NULL);
}