#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <inttypes.h>
#include <time.h>
#define MAX 100

int main()
{
	pid_t parent = getpid();

	// Accept the dimensions of the matrices
	int dim[2][2];
	printf("Enter dimensions (m,n) for Matrix-1:\n");
	scanf("%d %d",&dim[0][0],&dim[0][1]);

	printf("Enter dimensions (m,n) for Matrix-2:\n");
	scanf("%d %d",&dim[1][0],&dim[1][1]);

	// Do dimensionality check
	if(dim[0][1] != dim[1][0])
	{
		printf("Matrices of given dimensions cannot be multiplied.\n");
		return 0;
	}

	// Accept the matrices
	int A[dim[0][0]][dim[0][1]];
	int B[dim[1][0]][dim[1][1]];
	int i,j,k;

	printf("Enter the elements of Matrix-1\n");
	for(i=0;i<dim[0][0];++i)
		for(j=0;j<dim[0][1];++j)
			scanf("%d",&A[i][j]);

	printf("Enter the elements of Matrix-2\n");
	for(i=0;i<dim[1][0];++i)
		for(j=0;j<dim[1][1];++j)
			scanf("%d",&B[i][j]);

	// declare product matrix
	int product[dim[0][0]][dim[1][1]];

	// Have to create processes for each row and each column. - m*n processes
	pid_t pid[dim[0][0]];
	pid_t pid_col[dim[0][1]];
	for(i=0;i<dim[0][0];++i)
	{
		if(getpid() == parent)
		{
			pid[i] = vfork();
			if(pid[i] == 0)
			{
				for(j=0;j<dim[0][1];++j)
				{
					pid_col[j] = vfork();
					if(pid_col[j] == 0)
						break;
					// printf("Count Col\n");
				}
				break;
			}
			// printf("Count row\n");
		}
	}

// Start the multiplication process
int l,m,p;
for(l=0;l<dim[0][0];++l)
{	
	if(pid[l] == 0)
	{
		for(m=0;m<dim[1][1];++m)
		{
			if(pid_col[m] == 0)
			{
				product[l][m] = 0;
				for(p=0;p<dim[0][1];++p)
					product[l][m] = product[l][m] + A[l][p] * B[p][m];
				exit(0);
			}
			else
				wait(NULL);			
		}
		exit(0);
	}
	else
		wait(NULL);
}
while(wait(NULL) > 0);

// Print the product matrix
printf("The product of the matrix multiplication is:\n");
for(i=0;i<dim[0][0];++i)
{
	for(j=0;j<dim[1][1];++j)
		printf("%d ",product[i][j]);
	puts("");
}

return 0;	
}