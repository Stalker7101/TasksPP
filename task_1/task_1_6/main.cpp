#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>

int main(int argc, char **argv)
{
    for (int i = 1; i < argc; i++)
    {
        int status;
        if(fork() > 0)
	{
            wait(&status); /*процесс-предок ожидает сообщения от процесса-потомка о завершении */
            printf("process-father\n");
            continue;
        }
        execlp(argv[i], argv[i], 0);
	    /*попадем сюда при неуспехе exec()*/
        return -1;
    }
    return 0;
}

