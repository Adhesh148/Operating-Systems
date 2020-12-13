#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>

int main ()
{
printf("This will be printed once.\n");   // Print 1
fork();
printf("This will be printed twice.\n");   // Print 2
fork();
printf("This will be printed four times.\n");  // Print 3
fork();
printf("This will be printed eight times.\n");    // Print 4
return 0;
}
