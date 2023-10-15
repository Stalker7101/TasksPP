#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

int main(int argc, char **argv)
{
    char ch, first, last;
    int pid;

    if((pid = fork()) > 0)
    { /*процесс-предок*/
        first ='A'; last ='Z';
    } 
    else 
    { /*процесс-потомок*/
        first ='a'; last ='z';
    }

    for (ch = first; ch <= last; ch++)
    {
        write(1,&ch,1);
    }

    return 0;
}