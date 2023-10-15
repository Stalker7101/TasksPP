#include <unistd.h>                 // Provides API for POSIX(or UNIX) OS for system calls
#include <stdio.h>                  // Standard I/O Routines
#include <stdlib.h>                 // For exit() and rand()
#include <pthread.h>                // Threading APIs
#include <semaphore.h>              // Semaphore APIs
#define MAX_CHAIRS 10               // No. of chairs in waiting room
#define CUT_TIME 1                  // Hair Cutting Time 1 second
#define NUM_BARB 2                  // No. of barbers
#define MAX_CUST 30                 // Maximum no. of customers for simulation

sem_t customersSem;                 // Semaphore
sem_t barbersSem;                   // Semaphore
sem_t mutex;                        // Semaphore for providing mutially exclusive access

int numberOfFreeSeats = MAX_CHAIRS; // Counter for Vacant seats in waiting room
int seatPocket[MAX_CHAIRS];
int sitHereNext = 0;                // Index for next legitimate seat
int serveMeNext = 0;                // Index to choose a candidate for cutting hair
int maxBarberSleep = 5;             // max barber sleep time (in order to avoid infinite waiting)
static int count = 0;               // Counter of No. of customers
pthread_t barberTids[NUM_BARB];
pthread_t customerTids[MAX_CUST];


int barbers[NUM_BARB];
int customers[MAX_CUST];

void* barber_thread(void *tmp);     // Thread Function
void* customer_thread(void *tmp);   // Thread Function
void wait();                        // Randomized delay function

//----------------------------------------------------------------------------//
int main()
{
    int i, status = 0;
    sem_init(&customersSem, 0, 0);
    sem_init(&barbersSem, 0, 0);
    sem_init(&mutex, 0, 1);

    for (i = 0; i < NUM_BARB; ++i)
    {
        barbers[i] = i;
        status = pthread_create(
            &barberTids[i],
            NULL,
            barber_thread,
            &barbers[i]);

        sleep(1);

        if(status!=0)
            perror("No Barber Present... Sorry!!\n");
    }
    
    for (i = 0; i < MAX_CUST; ++i)
    {
        customers[i] = i;
        status = pthread_create(
            &customerTids[i],
            NULL,
            customer_thread,
            &customers[i]);

        wait();

        if(status != 0)
            perror("No Customers Yet!!!\n");
    }


    for (i = 0; i < NUM_BARB; ++i)
        pthread_join(barberTids[i], NULL);

    for (i = 0; i < MAX_CUST; ++i)
        pthread_join(customerTids[i], NULL);

    return 0;
}

//----------------------------------------------------------------------------//
void* customer_thread(void *tmp) /* Customer Thread */
{
    int mySeat, B;
    sem_wait(&mutex);      // Lock mutex to protect seat changes
    int custNum = ++count; // Arrival of customer
    printf("Customer-%d[Id:%ld] Entered Shop. ", custNum, pthread_self());

    if (numberOfFreeSeats > 0)
    {
        --numberOfFreeSeats;
        printf("Customer %d sits in waiting Room.\n", custNum);
        sitHereNext = (++sitHereNext) % MAX_CHAIRS;
        mySeat = sitHereNext;
        seatPocket[mySeat] = custNum;
        sem_post(&mutex);        // Release the seat change mutex
        sem_post(&barbersSem);   // Call barber
        sem_wait(&customersSem); // Join queue of sleeping customers
        sem_wait(&mutex);        // Lock mutex to protect seat changes

        B = seatPocket[mySeat];  // Barber replaces customer PID with his
                                 // own PID and customer get it

        printf("Customer %d is getting haircut by barber %d .\n", custNum, B);

        numberOfFreeSeats++;     // Stand Up and Go to Barber Room
        sem_post(&mutex);        // Release the seat change mutex
    }
    else
    {
        // Release the mutex and customer leaves without haircut
        printf("Customer %d finds no seat & leaves.\n", custNum);
        sem_post(&mutex);
    }

    pthread_exit(0);
}

//----------------------------------------------------------------------------//
void* barber_thread(void *tmp) /* Barber Thread */
{
    int barbNum = *(int*)(tmp);
    int myNext, C;
    printf("Barber-%d[Id:%ld] Joins Shop. ", barbNum, pthread_self());

    for (int j = 0; j < MAX_CUST; ++j)
    {
        printf("Barber-%d Gone To Sleep.\n", barbNum);
        timespec barberMaxSleep;
        clock_gettime(CLOCK_REALTIME, &barberMaxSleep);
        barberMaxSleep.tv_sec += 5;
        sem_timedwait(&barbersSem, &barberMaxSleep); // Join queue of sleeping barbers
        sem_wait(&mutex);                            // Lock mutex to protect seat changes

        if (numberOfFreeSeats < MAX_CHAIRS)
        {
            serveMeNext = (++serveMeNext) % MAX_CHAIRS;
            myNext = serveMeNext;
            C = seatPocket[myNext];                  // Get selected customer count
            seatPocket[myNext] = barbNum;            // Leave own index for customer
            printf("Barber's customer %d \n", C);

            sem_post(&mutex);
            sem_post(&customersSem);                 // Call selected customer

            printf("Barber %d wakes up & is cutting hair of customer %d.\n", barbNum, C);
            sleep(CUT_TIME);
            printf("Barber-%d finishes with %d customer.\n", barbNum, C);
        }
        else
        {
            printf("No more customers.\n");
            sem_post(&mutex);
            pthread_exit(0);
        }
    }

    pthread_exit(0);
}

//----------------------------------------------------------------------------//
void wait() /* Generates random number between 50000 to 250000 */
{
    int x = rand() % (250000 - 50000 + 1) + 50000;
    srand(time(NULL));
    usleep(x); //usleep halts execution in specified miliseconds
}

// https://askdev.ru/q/algoritm-spyaschego-parikmahera-s-neskolkimi-parikmaherami-616137/
