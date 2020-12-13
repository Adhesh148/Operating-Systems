#include <bits/stdc++.h>
using namespace std;
#include <sys/types.h>
#include<sys/wait.h> 
#include <unistd.h>
#include <inttypes.h>
#define MAX 100

int rowSum(vector< vector<int> > A,int n);
int columnSum(vector< vector<int> > A,int n);
int diagonalSum(vector< vector<int> > A,int n);

int main()
{
	// Accpet the dimension of the square matrix
	int n;
	printf("Enter order of the matrix (n)\n");
	cin>>n;

	// Accept n*n square matrix
	vector< vector<int> > A(n,vector<int>(n,0));
	printf("Enter the elements of the matrix.\n");
	for(int i=0;i<n;++i)
		for(int j=0;j<n;++j)
			cin>>A[i][j];

	// create 3 processes to handle the row, column and diagonal sums concurrently
	int rstatus,cstatus,dstatus;
	pid_t pid_r = vfork();
	if(pid_r == 0)
	{
		rstatus = rowSum(A,n);
		exit(0);
	}
	else
	{
		pid_t pid_c = vfork();
		if(pid_c == 0)
		{
			cstatus = columnSum(A,n);
			exit(0);
		}
		else
		{
			dstatus = diagonalSum(A,n);
		}
	}

	while(wait(NULL) > 0);
	if(dstatus == cstatus && cstatus == rstatus && rstatus > 0 && dstatus > 0)
		printf("Given matrix is a Magic Square with Maigc Sum = %d\n",cstatus);
	else
		printf("Given matrix is NOT a Magic Square.\n");

	return 0;
}

int rowSum(vector< vector<int> > A,int n)
{
	int sumR[n];
	pid_t pidR[n];
	pid_t rowParent = getpid();

	// Create n process to fill the sumR array concurrently
	for(int i=0;i<n;++i)
	{
		if(getpid() == rowParent)
		{
			pidR[i] = vfork();
			if(pidR[i] == 0)
				break;
		}
	}

	// Here fill the sumR[i] using pidR[i] process concurrently
	for(int i=0;i<n;++i)
	{
		if(pidR[i] == 0)
		{
			sumR[i] = 0;
			for(int j=0;j<n;++j)
				sumR[i] += A[i][j];
			exit(0);
		}
	}
	// wait for all children processes to complete
	while(wait(NULL) > 0);

	// Here linearly check if all sumR[i] are the same
	for(int i=1;i<n;++i)
	{
		if(sumR[i] != sumR[i-1])
			return -1;
	}
	return sumR[0];
}

int columnSum(vector< vector<int> > A,int n)
{
	int sumC[n];
	pid_t pidC[n];
	pid_t colParent = getpid();

	// Create n process to fill the sumC array concurrently
	for(int i=0;i<n;++i)
	{
		if(getpid() == colParent)
		{
			pidC[i] = vfork();
			if(pidC[i] == 0)
				break;
		}
	}

	// Here fill the sumC[i] using pidC[i] process concurrently
	for(int i=0;i<n;++i)
	{
		if(pidC[i] == 0)
		{
			sumC[i] = 0;
			for(int j=0;j<n;++j)
				sumC[i] += A[j][i];
			exit(0);
		}
	}
	// wait for all children processes to complete
	while(wait(NULL) > 0);

	// Here linearly check if all sumC[i] are the same
	for(int i=1;i<n;++i)
	{
		if(sumC[i] != sumC[i-1])
			return -1;
	}
	return sumC[0];
}

int diagonalSum(vector< vector<int> > A,int n)
{
	// Create two processes
	int sumD[2];
	pid_t pidD = vfork();
	if(pidD == 0)
	{
		sumD[0] = 0;
		for(int i=0;i<n;++i)
			sumD[0] += A[i][i];
		exit(0); 
	}
	else if(pidD > 0)
	{
		sumD[1] = 0;
		for(int i=0;i<n;++i)
			sumD[1] += A[i][n-i-1];
	}
	// wait for child process to complete.
	wait(NULL);

	if(sumD[0]!= sumD[1])
		return -1;
	
	return sumD[0];
}