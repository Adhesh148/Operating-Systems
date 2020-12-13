/******************************************
* AUTHOR : AdheshR *
* Problem Description: Binary Search*
******************************************/
#include <stdio.h>
#include <stdlib.h>
#define MAX 1000

int binarySearch(int arr[], int l, int r, int x) ;

int main(int argc, char *argv[])
{
	// Define the array and length and key x
	int arr[] = {1,2,3,4,5,6};
	int n = 6;
	int x = 6;

	int pos = binarySearch(arr,0,n-1,x);
	if(pos >= 0)
		printf("Position of %x in the array is %d\n",x,pos);
	else
		printf("%d not found in given array\n",x);

	return 0;
}

// Define the binary search function
int binarySearch(int arr[], int l, int r, int x) 
{ 
    if (r >= l) { 
        int mid = l + (r - l) / 2; 
 
        if (arr[mid] == x) 
            return mid; 
 
        if (arr[mid] > x) 
            return binarySearch(arr, l, mid - 1, x); 
  
        return binarySearch(arr, mid + 1, r, x); 
    } 
    return -1; 
} 