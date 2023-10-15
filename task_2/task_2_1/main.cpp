#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdio.h>

#define N 5                      /* количество философов */
#define LEFT (phi+4)%N           /* номер легого соседа для i-ого философа */
#define RIGHT (phi+1)%N          /* номер правого соседа для i-ого философа*/
#define THINKING 0               /* философ думает */
#define HUNGRY 1                 /* философ голоден */
#define EATING 2                 /* философ ест */

int phil[N] = { 0, 1, 2, 3, 4 }; /* массив философов */
int state[N]{};                  /* массив состояний философов */
sem_t mutex;                     /* семафор для критической секции */
sem_t s[N];                      /* по одному семафору на философа */

//----------------------------------------------------------------------------//
void think(int phi)              /* философ думает */
{
    sleep(1);
}

//----------------------------------------------------------------------------//
void eat(int phi)                /* философ ест */
{
    printf("Philosopher %d is Eating\n", phi + 1);
    sleep(1);
}

//----------------------------------------------------------------------------//
void test(int phi)               /* i : номер философа от 0 до N-1 */
{ 
    if ((state[phi] == HUNGRY)
        && (state[LEFT] != EATING)
        && (state[RIGHT] != EATING))
    {
        state[phi] = EATING;
        sem_post(&s[phi]);
    }
}

//----------------------------------------------------------------------------//
void take_forks(int phi)         /* i : номер философа от 0 до N-1 */
{
    sem_wait(&mutex);            /* вход в критическую секцию */
    state[phi] = HUNGRY;         /* записываем, что i-ый философ голоден */
    test(phi);                   /* попытка взять обе вилки */
    sem_post(&mutex);            /* выход из критической секции */
    sem_wait(&s[phi]);           /* блокируемся, если вилок нет */
}

//----------------------------------------------------------------------------//
void put_forks(int phi)          /* i : номер философа от 0 до N-1 */
{ 
    sem_wait(&mutex);            /* вход в критическую секцию */
    state[phi] = THINKING;       /* философ закончил есть */
    test(LEFT);                  /* проверить может ли левый сосед сейчас есть */
    test(RIGHT);                 /* проверить может ли правый сосед сейчас есть */
    sem_post(&mutex);            /* выход из критической секции */
}

//----------------------------------------------------------------------------//
void* philosopher(void* ph)      /* i : номер философа от 0 до N-1 */
{
    int phi = *(int*)ph;
    for (int i = 0; i < 3; ++i)  /* цикл на 10 повторений */    
    { 
        think(phi);             /* философ думает */
        take_forks(phi);        /* философ берет обе вилки или блокируется */
        eat(phi);               /* философ ест */
        put_forks(phi);         /* философ освобожает обе вилки */
    }
    return nullptr;
}

//----------------------------------------------------------------------------//
int main()
{
    pthread_t thread_id[N];
    sem_init(&mutex, 0, 1);

    for (int i = 0; i < N; ++i)
    {
        sem_init(&s[i], 0, 0);
    }

    for (int i = 0; i < N; ++i)
    {
        pthread_create(
            &thread_id[i],
            NULL,
            philosopher,
            &phil[i]);
    }

    for (int i = 0; i < N; ++i)
    {
        pthread_join(
            thread_id[i],
            NULL);
    }

    return 0;
}