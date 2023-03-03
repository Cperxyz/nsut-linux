#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <semaphore.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>



#define COUNT 5

sem_t *mutex;

sem_t* Semaphore(char *SEM_NAME, int count)
{
    sem_unlink(SEM_NAME);
    return sem_open(SEM_NAME,
                        O_CREAT, S_IRUSR | S_IWUSR, count);
}

int getvalue(sem_t* mutex)
{
    int value;
    sem_getvalue(mutex, &value);
    return value;
}

void set(sem_t* mutex)
{
    if (getvalue(mutex) == 0)
        sem_post(mutex);
}

void unset(sem_t* mutex)
{
    if (getvalue(mutex) == 1)
        sem_wait(mutex);
}

int main()
{
    sem_t* produced = Semaphore("producer", 0);
    //Producer
    if(!fork())
    {
        int i = COUNT;
        while(i--)
        {
            while(getvalue(produced));
            printf("Produced\n");
            set(produced);
        }
        exit(0);
    }
    //Consumer
    if(!fork())
    {
        int i = COUNT;
        while(i--)
        {
            while(!getvalue(produced));
            printf("Consumed\n");
            unset(produced);
        }
        exit(0);
    }
    int status = 0;
    while(wait(&status) > 0);
    return 0;
}
