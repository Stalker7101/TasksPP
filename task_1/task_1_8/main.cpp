#include <unistd.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char **argv)
{
    char s[] = "channel";
    char buf[80];
    int pipes[2];
    pipe(pipes);
    write(pipes[1], s, strlen(s) + 1);
    read(pipes[0], buf, strlen(s) + 1);
    close(pipes[0]);
    close(pipes[1]);
    printf("%s\n", buf);
    return 0;
}