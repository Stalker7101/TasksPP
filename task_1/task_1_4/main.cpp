#include <unistd.h>
#include <stdio.h>

int main(int argc, char **argv)
{
    printf("Не получилось передать g++ правильный "
           "относительный путь к файлу. Вставил ls.");
    execl("/bin/ls", "ls", "-l", (char*)0);
}
