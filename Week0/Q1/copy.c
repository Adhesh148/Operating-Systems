/**************************************************************************************
* AUTHOR : AdheshR*
* Question No.: Q1.i. * 
* Problem Statement: Simulate the behavior of cp command in linux. (Just the basic version.)*
**************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// STATUS tells result of the copy operation.
enum STATUS
{
	SRC_INC,
	SUCCESS,
	FAILURE
};

enum STATUS fileCopy(char* src, char* dest);

int main(int argc,char* argv[])
{
	if(argc < 3)
		printf("Insufficient Arguments Passed.\n");
	else if(argc == 3)
	{
		// Get source and destination and pass to fileCopy() function.
		char* src = argv[1];
		char* dest = argv[2];
		enum STATUS status = fileCopy(src,dest);
		// Print the status of copy.
		if(status == SRC_INC)
			printf("Source Not Found.\n");
		else if(status == SUCCESS)
			printf("Copy Successful.\n");
		else
			printf("Error.\n");
	}
	else
		printf("Too Many Arguments.\n");
	return 0;
}

enum STATUS fileCopy(char* src, char* dest)
{
	FILE *fp;
	// Read binaries to handle any format of file.
	fp = fopen(src,"rb");

	if(fp == NULL)
		return SRC_INC;
	else
	{
		// get the size of the fp by seeking to END OF FILE 
		fseek(fp,0,SEEK_END);
		long int len = ftell(fp);
		fseek(fp,0,SEEK_SET);

		// create a buffer of input size(bytes)
		unsigned char buffer[len];

		// read into buffer
		fread(buffer,sizeof(buffer),1,fp);

		// write binary to output
		FILE *fp_w;
		fp_w = fopen(dest,"wb");
		fwrite(buffer,sizeof(buffer),1,fp_w);
		fclose(fp);
		return SUCCESS;
	}
	return FAILURE;
}