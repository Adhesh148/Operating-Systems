/**************************************************************************************
* AUTHOR : AdheshR*
* Question No.: Q1.ii. * 
* Problem Statement: Simulate the behavior of rm command in linux. (Deletion of multiple files possible)*
**************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int deleteFiles(char** files,int count);

int main(int argc, char *argv[])
{
	if(argc < 2)
		printf("Insufficient Arguments Passed.\n");
	else
	{
		int status = deleteFiles(argv,argc);
		// status is -1 if none of the files can be deleted.
		if(status == -1)
			printf("invalid arguments\n");
	}
	
	return 0;
}

int deleteFiles(char** files,int count)
{
	// Maintain a flag to return as status
	int flag = -1;

	// Start a loop to get all filenames in the argument passed
	int i=1;
	char *fileName;
	while(i<count)
	{
		// extract next fileName
		fileName = files[i];

		if(remove(fileName) == 0)
		{
			++flag;
			printf("removed '%s\'\n",fileName);
		}
		else
			printf("cannot remove \'%s\': No such file or directory\n",fileName);
		i = i+1;	
	}
	return flag;
}

