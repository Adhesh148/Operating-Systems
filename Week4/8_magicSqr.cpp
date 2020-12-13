/******************************************
* AUTHOR : AdheshR *
******************************************/
#include <bits/stdc++.h>
#include <sys/types.h>
#include <unistd.h>
#include <inttypes.h>
#include <string.h>
#include <math.h>
#include <sys/wait.h>
using namespace std;
#define LL long long
#define MAX 100
#define MOD 1000000007
#define dd double

void generateOddSquare(int n,vector<vector <int > > &magicSqr,int row, int col, int offset);
void generateDoublyEvenSqr(int n,vector<vector <int > > &magicSqr);
void generateSinglyEvenSqr(int n,vector<vector <int > > &magicSqr);

int main()
{
	int n;
	printf("Enter the order of \n");
	scanf("%d",&n);

	vector<vector <int > > magicSqr(n,vector<int > (n,0));

	if(n%2 == 1)
		generateOddSquare(n,magicSqr,0,0,0);
	else if(n%4 == 0)
		generateDoublyEvenSqr(n,magicSqr);
	else if(n%4 == 2)
		generateSinglyEvenSqr(n,magicSqr);

	printf("Magic Square is\n");
	for(int i=0;i<n;++i)
	{
		for(int j=0;j<n;++j)
			printf("%2d ",magicSqr[i][j]);
		puts("");
	}	
}

void generateOddSquare(int n, vector<vector <int > > &magicSqr,int row, int col, int offset)
{
	// Let us generate the magic square of odd order n*n using vfork() - here let's split the process in two
	pid_t pid = vfork();
	if(pid == 0)
	{
		// In the child process, lets fill from 1 to floor(n^2/2) in the matrix
		int i = row + 0,j = col + ceil(n/2);
		int num = 1 + offset,target = (n*n)/2 + offset;
		while(num <= target)
		{
			magicSqr[i][j] = num;
			++num;

			// Check for boundary or cell-fill condition
			if(magicSqr[(((i-1)%n + n)%n + row)][(j+1)%n + col] == 0)
			{
				i = ((i-1)%n + n)%n + row;
				j = (j+1)%n + col;
			}
			else
				i = (i + 1)%n + row;
		}
		exit(0);
	}
	else if(pid > 0)
	{
		// In the parent process, let us fill from n^2/2 + 1 to n^2.
		int i = ceil(n/2) + row,j = ceil(n/2) + col;
		int num = (n*n)/2 + 1 + offset,target = n*n + offset;
		while(num <= target)
		{
			magicSqr[i][j] = num;
			++num;

			// Check for boundary or cell-fill condition
			if(magicSqr[(((i-1)%n + n)%n) + row][(j+1)%n + col] == 0)
			{
				i = ((i-1)%n + n)%n + row;
				j = (j+1)%n + col;
			}
			else
				i = (i + 1)%n + row;
		}
	}
	else
		perror("Process could not be created.");

	wait(NULL);
}

void generateDoublyEvenSqr(int n,vector<vector <int > > &magicSqr)
{
	int i,j;
	// First let us fill the matrix
	for(i=0;i<n;++i)
		for(j=0;j<n;++j)
			magicSqr[i][j] = (n*i) + j + 1;

	pid_t pid = vfork();
	if(pid == 0)
	{
		pid_t pid_0 = vfork();
		if(pid_0 == 0)
		{
			// Bottom-Right
			for (i = 3 * n/4; i < n; i++) 
				for ( j = 3 * n/4; j < n; j++) 
			  		magicSqr[i][j] = (n*n + 1) - magicSqr[i][j]; 

			// Center of Matrix
			for (i = n/4; i < 3 * n/4; i++) 
				for (j = n/4; j < 3 * n/4; j++) 
					magicSqr[i][j] = (n*n + 1) - magicSqr[i][j]; 

			exit(0);
		}
		else if(pid_0 > 0)
		{
			// Bottom Left
			for (i = 3 * n/4; i < n; i++) 
				for (j = 0; j < n/4; j++) 
			        magicSqr[i][j] = (n*n+1) - magicSqr[i][j]; 
		}
		wait(NULL);
		exit(0);
	}
	else if(pid > 0)
	{
		pid_t pid_1 = vfork();
		if(pid_1 == 0)
		{
			// Top-Right Corner
			for(i=0;i<n/4;++i)
				for(j=3*(n/4);j<n;++j)
					magicSqr[i][j] = (n*n + 1) - magicSqr[i][j];
			exit(0);
		}
		else if(pid_1 > 0)
		{
			// Top-Left Corner
			for(i=0;i<n/4;++i)
				for(j=0;j<n/4;++j)
					magicSqr[i][j] = (n*n + 1) - magicSqr[i][j];
		}
		wait(NULL);
	}
	else
		perror("Process could not be created.");
	wait(NULL);
}

void generateSinglyEvenSqr(int n,vector<vector <int > > &magicSqr)
{
	// Divide the matrix into four sub matrices of size
	int size = n/2;

	// Do each quarter generation in a process

	pid_t pid[6];
	pid[0] = vfork();
	if(pid[0] == 0)
	{
		pid[1] = vfork();
		if(pid[1] == 0)
		{
			generateOddSquare(size,magicSqr,size,size,size*size);
			exit(0);
		}
		else
			generateOddSquare(size,magicSqr,size,0,3*(size*size));
		wait(NULL);
		exit(0);
	}
	else
	{
		pid[2] = vfork();
		if(pid[2] == 0)
		{
			generateOddSquare(size,magicSqr,0,size,2*(size*size));
			exit(0);
		}
		else
			generateOddSquare(size,magicSqr,0,0,0);
		wait(NULL);	
	}
	wait(NULL);

	// Now let us do the swaps necessary - in four processes
	int k = (n-2)/4;

	pid[3] = vfork();
	if(pid[3] == 0)
	{
		pid[4] = vfork();
		if(pid[4] == 0)
		{
			// kth column - all but middle
			for(int i=0;i<size;++i)
				if(i!=size/2)
					swap(magicSqr[i][k-1],magicSqr[i+size][k-1]);
			exit(0);
		}
		else
		{
			// Swap middle Element of k+1 th col
			swap(magicSqr[size/2][k],magicSqr[size/2 + size][k]);
		}
		wait(NULL);
		exit(0);
	}
	else
	{
		pid[5] = vfork();
		if(pid[5] == 0)
		{
			// Last (k-1) columns
			for(int i=n-k+1;i<n;++i)
				for(int j=0;j<size;++j)
					swap(magicSqr[j][i],magicSqr[j+size][i]);
			exit(0);
		}
		else
		{
			// First (k-1) columns
			for(int i=0;i<k-1;++i)
				for(int j=0;j<size;++j)
					swap(magicSqr[j][i],magicSqr[j+size][i]);
		}
		wait(NULL);
	}
	wait(NULL);
}