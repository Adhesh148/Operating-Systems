//
//  thread_hist.c
//  
//
//  Created by Aparajith Raghuvir on 01/11/20.
//


#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include<pthread.h>
#include<string.h>
#include<unistd.h>
#define LEN 128
pthread_mutex_t lock;
void * runner(void * arg);

int hash[256] = {0};
int main()
{
    pthread_mutex_init(&lock, NULL);
    FILE *fptr;
    fptr = fopen("input", "rb");
    char buffer[LEN];

    fseek(fptr, 0, SEEK_END);//set pointer to end of file
    long long len = ftell(fptr);//get file length
    fseek(fptr, 0, SEEK_SET);//set pointer back to beginning of the file

    int thread_knt = (int)ceil(((float)len) / LEN);
    pthread_t t[thread_knt];
    long long counter = 0;
    int thread_counter = 0;
    do
    {
        int read_knt = fread(buffer, 1, sizeof(buffer), fptr);
        char temp[LEN];
        strcpy(temp, buffer);
        pthread_create(&t[thread_counter], NULL, runner, temp);
        bzero(buffer, sizeof(buffer));
        counter += read_knt;
        thread_counter++;

    }while(counter < len && thread_counter < thread_knt);

	int i;
    for(i = 0; i < thread_knt; i++)
        pthread_join(t[i], NULL);
    pthread_mutex_destroy(&lock);
    printf("Histogram based on extended ASCII set is \n");
    if(hash[0])
        printf("Number of occurences of NULL : %d\n", hash[0]);
    if(hash[7])
        printf("Number of occurences of Bell Character : %d\n", hash[7]);
    if(hash[8])
        printf("Number of occurences of Backspace: %d\n", hash[8]);
    if(hash[9])
        printf("Number of occurences of Horizontal Tab : %d\n", hash[9]);
    if(hash[10])
        printf("Number of occurences of New Line : %d\n", hash[10]);
    if(hash[11])
        printf("Number of occurences of Vertical Tab : %d\n", hash[11]);
    if(hash[13])
        printf("Number of occurences of Carriage Return : %d\n", hash[13]);
    if(hash[32])
        printf("Number of occurences of Space : %d\n", hash[32]);
    for(i = 1; i < 9; i++)
        if(hash[i] != 0)
            printf("Number of occurences of %c : %d\n", i, hash[i]);
    for(i = 14; i < 32; i++)
        if(hash[i])
            printf("Number of occurences of %c : %d\n", i, hash[i]);
    for(i = 33; i < 256; i++)
        if(hash[i] != 0)
            printf("Number of occurences of %c : %d\n", i, hash[i]);

    fclose(fptr);
}

void * runner(void * arg)
{
    char *buffer = (char *)arg;
    pthread_mutex_lock(&lock);
	int i=0;
    for(i = 0; i < strlen(buffer); i++)
        hash[(int)buffer[i]]++;
    pthread_mutex_unlock(&lock);
    pthread_exit(NULL);
}
