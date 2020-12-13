/******************************************
* AUTHOR : AdheshR *
* Problem Description: *
*****************************************/
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <inttypes.h>
#include <fcntl.h>
#include <pthread.h>
#define MAX 100

#include<semaphore.h>
sem_t s;

int main()
{
    sem_init(&s, 0, 0);
    int c = vfork();
    if(c == 0)
    {
        sem_wait(&s);
        printf("Child\n");
        // sem_post(&s);  
        exit(0);
    }
    else
    {
        // sem_wait(&s);
        printf("parent\n");
        sem_post(&s);
    }
    return 0;
//    pthread_exit("OS 2018 Logged Out ..\n");
//    printf("fg command ?");
}