/* процесс-клиент*/
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char **argv)
{
    int fd;
    int pid = getpid();
    fd = open("fifo", O_RDWR);
    write(fd, &pid, sizeof(int));
    close(fd);
    return 0;
}
