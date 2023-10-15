#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char **argv)
{
    int pid;
    if ((pid = fork()) == 0)
    {
        printf("Процесс-потомок.\n");
        execl("/bin/ls", "ls", "-l", (char*)0);
    }
    else if (pid > 0)
    {
        printf("Процесс-предок.\n");
    }
    else
    { 
        printf("Ошибка при вызове exec().\n");
    }
}

