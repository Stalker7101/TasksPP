#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

#define CONSUMER_NUM 2
#define PRODUCER_NUM 2

int ready = 0;
int producers[PRODUCER_NUM] = { 0, 1 };
int consumers[CONSUMER_NUM] = { 0, 1 };
pthread_t tids[CONSUMER_NUM+PRODUCER_NUM];
pthread_mutex_t mutex;
pthread_cond_t has_product;

//----------------------------------------------------------------------------//
void* producer(void* arg)
{
    int i = *(int*)arg;
    for(int j = 0; j < 5; ++j) 
    {
        pthread_mutex_lock(&mutex);
        ready++;
        printf("producer %d, produce product\n", i);
        pthread_cond_signal(&has_product);
        printf("producer %d, signal\n", i);
        pthread_mutex_unlock(&mutex);
        sleep(1);
    }
    return nullptr;
}

//----------------------------------------------------------------------------//
void* consumer(void* arg)
{
    int i = *(int*)arg;
    for(int j = 0; j < 5; ++j)
    {
        pthread_mutex_lock(&mutex);
        while (ready == 0)
        {
            printf("%d consumer wait\n", i);
            pthread_cond_wait(&has_product, &mutex);
        }
        ready--;
        printf("%d consume product\n", i);
        pthread_mutex_unlock(&mutex);
        sleep(1);
    }
    return nullptr;
}

//----------------------------------------------------------------------------//
int main()
{
    pthread_mutex_init(&mutex,NULL);
    pthread_cond_init(&has_product,NULL);

    int i;

    for(i = 0; i < PRODUCER_NUM; ++i)
        pthread_create(&tids[i], NULL, producer, &producers[i]);

    for(i = 0; i < CONSUMER_NUM; ++i)
        pthread_create(&tids[PRODUCER_NUM+i], NULL, consumer, &consumers[i]);

    sleep(10);

    for(i = 0; i < CONSUMER_NUM + PRODUCER_NUM; ++i)
        pthread_join(tids[i],NULL);

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&has_product);

    return 0;
}

