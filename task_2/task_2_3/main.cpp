#include <stdio.h>
#include <unistd.h>
#include <malloc.h>
#include <stdlib.h>
#include <pthread.h>

#define WRITERS_NUM 3
#define READERS_NUM 5

pthread_rwlock_t rwlock;
int count;
pthread_t writers_tid[WRITERS_NUM];
pthread_t readers_tid[READERS_NUM];
int writers[WRITERS_NUM] = { 0, 1, 2 };
int readers[READERS_NUM] = { 0, 1, 2, 3, 4 };

//----------------------------------------------------------------------------//
void* route_write(void *arg)
{
    int i = *(int*)arg;
    for (int j = 0; j < 5; ++j)
    {
        int t = count;
        pthread_rwlock_wrlock(&rwlock);
        printf("route_write:%d, %#lx, count=%d, ++count=%d\n",
               i, pthread_self(), t, ++count);
        pthread_rwlock_unlock(&rwlock);
        sleep(1);
    }
    return nullptr;
}

//----------------------------------------------------------------------------//
void* route_read(void *arg)
{
    int i = *(int*)arg;
    for (int j = 0; j < 5; ++j)
    {
        pthread_rwlock_rdlock(&rwlock);
        printf("route_read:%d, %#lx, count=%d\n",
               i, pthread_self(), count);
        pthread_rwlock_unlock(&rwlock);
        sleep(1);
    }
    return nullptr;
}

//----------------------------------------------------------------------------//
int main()
{
    int i;
    pthread_rwlock_init(&rwlock, NULL);

    for(i = 0; i < WRITERS_NUM; ++i)
    {
        pthread_create(&writers_tid[i], NULL, route_write, &writers[i]);
    }

    for(i = 0; i < READERS_NUM; ++i)
    {
        pthread_create(&readers_tid[i], NULL, route_read, &readers[i]);
    }

    for(i = 0; i < WRITERS_NUM; ++i)
        pthread_join(writers_tid[i], NULL);

    for(i = 0; i < READERS_NUM; ++i)
        pthread_join(readers_tid[i], NULL);

    pthread_rwlock_destroy(&rwlock);
    return 0;
}
