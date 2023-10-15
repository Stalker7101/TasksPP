#include <unistd.h>
#include <stdio.h>

int main(int argc, char **argv)
{
    execl("/bin/ls", "ls", "-l", (char*)0);
    /* или execlp("ls", "ls", "-l", (char*)0); */
    printf("это напечатается в случае неудачного обращения к"
    "предыдущей функции, к примеру, если не был найден файл ls\n");
}