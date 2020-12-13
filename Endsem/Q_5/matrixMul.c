/******************************************
* AUTHOR : AdheshR *
* Problem Description: Matrix Multiplication*
******************************************/
#include <stdio.h>
#include <stdlib.h>
#define MAX 1000
#define N 4

void matrixMul(int matA[][N], int matB[][N], int matC[][N]);

int main(int argc, char *argv[])
{
	// Define two matrices of N*N
	int matA[N][N] = { { 1, 1, 1, 1 },
                       { 2, 2, 2, 2 },
                       { 3, 3, 3, 3 },
                       { 4, 4, 4, 4 } };
 
    int matB[N][N] = { { 1, 1, 1, 1 },
                       { 2, 2, 2, 2 },
                       { 3, 3, 3, 3 },
                       { 4, 4, 4, 4 } };

    // Define result matrix
    int product[N][N];
    matrixMul(matA, matB, product);

    // Print the product matrix
    int i,j;
    for(i=0;i<N;++i)
    {
    	for(j=0;j<N;++j)
    		printf("%d ",product[i][j]);
    	puts("");
    }

	return 0;
}

// Define matrix multiplication function
void matrixMul(int matA[][N], int matB[][N], int product[][N])
{
	int i,j,k;
	for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            product[i][j] = 0;
            for (k = 0; k < N; k++)
                product[i][j] += matA[i][k] * matB[k][j];
        }
    }
}