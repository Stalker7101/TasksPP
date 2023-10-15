#include <sys/types.h>
#include <signal.h>
#include <stdio.h>

int count = 1;

void SigHndlr (int s)
{ /* обработчик сигнала */
    printf("\n I got SIGINT %d time(s) \n", ++count);
    if (count == 5) signal (SIGINT, SIG_DFL); /* ставим обработчик сигнала по умолчанию */
    else signal (SIGINT, SigHndlr); /* восстанавливаем обработчик сигнала */
}

int main(int argc, char **argv)
{
    signal (SIGINT, SigHndlr); /* установка реакции на сигнал */
    printf("Время чтобы послать сигнал процессу 3 секунд.");
    sleep(3); /*"тело программы" */
    return 0;
}

