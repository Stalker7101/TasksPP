#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

int main(int argc, char **argv)
{
    /*печать PID текущего процесса и PID процесса-предка */
    printf("PID=%d; PPID=%d \n", getpid(), getppid());
    fork(); /* создаем копию */
    /* и снова печаем PID */
    printf("PID=%d; PPID=%d \n", getpid(), getppid());
    return 0;
}

