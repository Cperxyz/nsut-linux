#include <bits/stdc++.h>
#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <semaphore.h>
#include <stdio.h>
#include <fcntl.h>

class Semaphore
{
private:
    bool counting;
    sem_t *mutex;

public:
    Semaphore(char *SEM_NAME, bool counting, int count = 1)
    {
        this->counting = counting;
        sem_unlink(SEM_NAME);
        mutex = sem_open(SEM_NAME,
                         O_CREAT, S_IRUSR | S_IWUSR, count);
    }
    void incr()
    {
        sem_post(mutex);
    }
    void decr()
    {
        sem_wait(mutex);
    }
    void set()
    {
        if (!(this->counting) && getvalue() == 0)
            incr();
    }
    void unset()
    {
        if (!(this->counting) && getvalue() == 1)
            decr();
    }
    int getvalue()
    {
        int value;
        sem_getvalue(mutex, &value);
        return value;
    }
};

using namespace std;

#define endl '\n'

int main()
{
    //Shared Memory Allocation
    key_t key_arr = ftok("/home/rohan/Desktop/shmfile_arr1", 65);

    int shmid_arr = shmget(key_arr, 13 * sizeof(int), 0666 | IPC_CREAT);

    int *arr = (int *)shmat(shmid_arr, (void *)0, 0);    

    cout << "Enter 8 elements" << endl;
    for (int i = 1; i <= 8; i++)
    {
        cin >> arr[i];
    }
    
    Semaphore SEM_BEGIN("begin", false, 0),
        SEM_NEXTPASS("nextpass", false, 0),
        SEM_PASSCOUNT("passcount", true, 0),
        SEM_READCOUNT("readcount", true, 4),
        SEM_EXECCOUNT("execcount", true, 4),
        SEM_WRITECOUNT("writecount", true, 4),
        SEM_NOOFWAITINGPROCESSES("noofwaitingprocesses", true, 0);
        
    // Process 1
    if (!fork())
    {
        int *arr = (int *)shmat(shmid_arr, (void *)0, 0);
        while (!SEM_BEGIN.getvalue())
            ;
        while (SEM_PASSCOUNT.getvalue() < 8)
        {
            int i = 1, j = 2;
            // READ
            bool odd = (SEM_PASSCOUNT.getvalue() + 1) & 1;
            if (!odd)
            {
                i++, j++;
            }
            int tmp1 = arr[i];
            int tmp2 = arr[j];
            // READCOUNT--
            SEM_READCOUNT.decr();
            // wait while READCOUNT > 0
            while (SEM_READCOUNT.getvalue() > 0);
            // EXECUTE
            if (tmp1 > tmp2)
                swap(tmp1, tmp2);
            // SET NEXTPASS to false
            SEM_NEXTPASS.unset();
            //  EXECCOUNT--
            SEM_EXECCOUNT.decr();
            //  wait while EXECCOUNT > 0
            while (SEM_EXECCOUNT.getvalue() > 0)
                ;
            // WRITE
            arr[i] = tmp1;
            arr[j] = tmp2;
            // WRITECOUNT--
            SEM_WRITECOUNT.decr();
            // wait while WRITECOUNT > 0
            while (SEM_WRITECOUNT.getvalue() > 0)
                ;
            // increment NoOfWaitingProcesses
            SEM_NOOFWAITINGPROCESSES.incr();
            while (!SEM_NEXTPASS.getvalue())
                ;
        }
        shmdt(arr);
        exit(0);
    }
    // Process 2
    if (!fork())
    {
        int *arr = (int *)shmat(shmid_arr, (void *)0, 0);
        while (!SEM_BEGIN.getvalue())
            ;
        while (SEM_PASSCOUNT.getvalue() < 8)
        {
            int i = 3, j = 4;
            // READ
            bool odd = (SEM_PASSCOUNT.getvalue() + 1) & 1;
            if (!odd)
            {
                i++, j++;
            }
            int tmp1 = arr[i];
            int tmp2 = arr[j];
            // READCOUNT--
            SEM_READCOUNT.decr();
            // wait while READCOUNT > 0
            while (SEM_READCOUNT.getvalue() > 0)
                ;
            // EXECUTE
            if (tmp1 > tmp2)
                swap(tmp1, tmp2);
            // SET NEXTPASS to false
            SEM_NEXTPASS.unset();
            //  EXECCOUNT--
            SEM_EXECCOUNT.decr();
            //  wait while EXECCOUNT > 0
            while (SEM_EXECCOUNT.getvalue() > 0)
                ;
            // WRITE
            arr[i] = tmp1;
            arr[j] = tmp2;
            // WRITECOUNT--
            SEM_WRITECOUNT.decr();
            // wait while WRITECOUNT > 0
            while (SEM_WRITECOUNT.getvalue() > 0)
                ;
            // increment NoOfWaitingProcesses
            SEM_NOOFWAITINGPROCESSES.incr();
            while (!SEM_NEXTPASS.getvalue())
                ;
        }
        shmdt(arr);
        exit(0);
    }
    // Process 3
    if (!fork())
    {
        int *arr = (int *)shmat(shmid_arr, (void *)0, 0);
        while (!SEM_BEGIN.getvalue())
            ;
        while (SEM_PASSCOUNT.getvalue() < 8)
        {
            int i = 5, j = 6;
            // READ
            bool odd = (SEM_PASSCOUNT.getvalue() + 1) & 1;
            if (!odd)
            {
                i++, j++;
            }
            int tmp1 = arr[i];
            int tmp2 = arr[j];
            // READCOUNT--
            SEM_READCOUNT.decr();
            // wait while READCOUNT > 0
            while (SEM_READCOUNT.getvalue() > 0)
                ;
            // EXECUTE
            if (tmp1 > tmp2)
                swap(tmp1, tmp2);
            // SET NEXTPASS to false
            SEM_NEXTPASS.unset();
            //  EXECCOUNT--
            SEM_EXECCOUNT.decr();
            //  wait while EXECCOUNT > 0
            while (SEM_EXECCOUNT.getvalue() > 0)
                ;
            // WRITE
            arr[i] = tmp1;
            arr[j] = tmp2;
            // WRITECOUNT--
            SEM_WRITECOUNT.decr();
            // wait while WRITECOUNT > 0
            while (SEM_WRITECOUNT.getvalue() > 0)
                ;
            // increment NoOfWaitingProcesses
            SEM_NOOFWAITINGPROCESSES.incr();
            while (!SEM_NEXTPASS.getvalue())
                ;
        }
        shmdt(arr);
        exit(0);
    }
    // Process 4
    if (!fork())
    {
        int *arr = (int *)shmat(shmid_arr, (void *)0, 0);
        while (!SEM_BEGIN.getvalue())
            ;
        while (SEM_PASSCOUNT.getvalue() < 8)
        {
            int i = 7, j = 8;
            // READ
            bool odd = (SEM_PASSCOUNT.getvalue() + 1) & 1;
            if (!odd)
            {
                i++, j++;
            }
            int tmp1 = odd ? arr[i] : 0;
            int tmp2 = odd ? arr[j] : 0;
            // READCOUNT--
            SEM_READCOUNT.decr();
            // wait while READCOUNT > 0
            while (SEM_READCOUNT.getvalue() > 0)
                ;
            // EXECUTE
            if (tmp1 > tmp2)
                swap(tmp1, tmp2);
            // SET NEXTPASS to false
            SEM_NEXTPASS.unset();
            //  EXECCOUNT--
            SEM_EXECCOUNT.decr();
            //  wait while EXECCOUNT > 0
            while (SEM_EXECCOUNT.getvalue() > 0)
                ;
            // WRITE
            if (odd)
            {
                arr[i] = tmp1;
                arr[j] = tmp2;
            }
            // WRITECOUNT--
            SEM_WRITECOUNT.decr();
            // wait while WRITECOUNT > 0
            while (SEM_WRITECOUNT.getvalue() > 0)
                ;
            // increment NoOfWaitingProcesses
            SEM_NOOFWAITINGPROCESSES.incr();
            while (!SEM_NEXTPASS.getvalue())
                ;
        }
        shmdt(arr);
        exit(0);
    }
    
    
    
    // Set begin semaphore to true
    SEM_BEGIN.set();
    
    for (int pass = 0; pass <= 7; pass++)
    {
        // wait while NoOfWaitingProcesses < 4
        while (SEM_NOOFWAITINGPROCESSES.getvalue() < 4)
            ;
        // incr passcount
        SEM_PASSCOUNT.incr();
        // set NoOfWaitingProcesses to 0
        while (SEM_NOOFWAITINGPROCESSES.getvalue() > 0)
            SEM_NOOFWAITINGPROCESSES.decr();
        // set READCOUNT to 4
        while (SEM_READCOUNT.getvalue() < 4)
            SEM_READCOUNT.incr();
        // set WRITECOUNT to 4
        while (SEM_WRITECOUNT.getvalue() < 4)
            SEM_WRITECOUNT.incr();
        // set EXECCOUNT to 4
        while (SEM_EXECCOUNT.getvalue() < 4)
            SEM_EXECCOUNT.incr();
        // set NEXTPASS to true
        SEM_NEXTPASS.set();
    }
    // display the arr
    for (int i = 1; i <= 8; i++)
    {
        cout << arr[i] << ' ';
    }
    cout << endl;

    shmdt(arr);

    shmctl(shmid_arr, IPC_RMID, NULL);
    return 0;
}
