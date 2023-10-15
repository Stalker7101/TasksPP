/* процесс-сервер*/
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char **argv)
{
    int fd; int pid;
    mkfifo("fifo", S_IFIFO | 0666); /* FIFO-файл с
    открытыми для всех правами доступа на чтение и
    запись*/
    /* открыли канал на чтение */
    fd = open("fifo", O_RDONLY | O_NONBLOCK);
    while(read(fd, &pid, sizeof(int)) == -1);
    printf("Server %d got message from %d !\n", getpid(), pid);
    close(fd);
    unlink("fifo");/*уничтожили именованный канал*/
    return 0;
}
