//
//  thread_pi.c
//  
//
//  Created by Aparajith Raghuvir on 29/10/20.
//

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>

#define thread_knt 16
#define N 100000007

float it = N / thread_knt;


void * pi_estimate(void *t);
int main(void)
{
    pthread_t thread[thread_knt];
    int temp[thread_knt];
	int i;
    for(i = 0; i < thread_knt; i++)
    {
        temp[i] = i;
        pthread_create(&thread[i], NULL, pi_estimate, &temp[i]);
    }

    void * status;
    float in = 0;
    for(i = 0; i < thread_knt; i++)
    {
        pthread_join(thread[i], &status);
        in += *(float *) status;
    }
    printf("Estimated value of pi is %f\n", 4*in / N);
}

void * pi_estimate(void *t)
{
    int tid = *(int*) t;

    float *in = (float *)malloc(sizeof(float));
    *in = 0;
    srand(time(NULL));
int i;
    for(i = 0; i < it; i++)
    {
        float x = rand() / (float) RAND_MAX;
        float y = rand() / (float) RAND_MAX;

        float result = sqrt(x * x + y * y);

        if(result <= 1)
            (*in)++;
    }
    pthread_exit((void *) in);
}
