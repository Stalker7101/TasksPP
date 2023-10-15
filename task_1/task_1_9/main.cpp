#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

char string[] = "hello world";

int main()
{
    int count;
    int to_chil[2]; // to_chil[0] – чтение данных из канала
    char buf[256];  // to_chil[1] – запись данных в канал
    pipe(to_chil);
    if (fork() == 0)
    { // внутри блока процесс-потомок
        close(to_chil[1]);
        for (;;)
	{
            if ((count = read(to_chil[0], buf, sizeof(buf))) == 0)
	    {
                exit(0);
	    }
            printf("read %s\n", buf);
	}
    }

    close(to_chil[0]); // родительский процесс
    for (int i = 0; i < 5; i++)
    {
        write(to_chil[1], string, strlen(string));
        printf("write %s\n", string);
    }

    return 0;
}

