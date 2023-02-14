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
    int shmid_arr = shmget(IPC_PRIVATE, 22 * sizeof(int), 0666 | IPC_CREAT);
    
    int *arr = (int *)shmat(shmid_arr, (void *)0, 0);    

    int n;
    cout<<"Enter the number of elements (must be <= 20 and even)"<<endl;
    cin>>n;
    while(n > 20 || n < 1 || n % 2 != 0)
    {
        cout<<"Re - enter the number of elements (must be <= 20 and even)"<<endl;
        cin>>n;
    }
    
    cout << "Enter " << n << " elements" << endl;
    for (int i = 1; i <= n; i++)
    {
        cin >> arr[i];
    }
    
    Semaphore SEM_BEGIN("begin", false, 0),
        SEM_NEXTPASS("nextpass", false, 0),
        SEM_PASSCOUNT("passcount", true, 0),
        SEM_READCOUNT("readcount", true, n / 2),
        SEM_EXECCOUNT("execcount", true, n / 2),
        SEM_WRITECOUNT("writecount", true, n / 2),
        SEM_NOOFWAITINGPROCESSES("noofwaitingprocesses", true, 0);
        
    for(int process = 1; process <= n / 2; process++)
    {
        if (!fork())
        {
            int *arr = (int *)shmat(shmid_arr, (void *)0, 0);
            while (!SEM_BEGIN.getvalue())
                ;
            bool lastProc = (process == n / 2);
            while (SEM_PASSCOUNT.getvalue() < n)
            {
                int i = 2 * process - 1, j = 2 * process;
                // READ
                bool odd = (SEM_PASSCOUNT.getvalue() + 1) & 1;
                if (!odd)
                {
                    i++, j++;
                }
                int tmp1, tmp2;
                if(lastProc)
                {
                    tmp1 = odd ? arr[i] : 0;
                    tmp2 = odd ? arr[j] : 0;
                }
                else
                {
                    tmp1 = arr[i];
                    tmp2 = arr[j];
                }
                SEM_READCOUNT.decr();
                while (SEM_READCOUNT.getvalue() > 0)
                    ;
                // EXECUTE
                if (tmp1 > tmp2)
                    swap(tmp1, tmp2);
                SEM_NEXTPASS.unset();
                SEM_EXECCOUNT.decr();
                while (SEM_EXECCOUNT.getvalue() > 0)
                    ;
                // WRITE
                if (!lastProc || odd)
                {
                    arr[i] = tmp1;
                    arr[j] = tmp2;
                }
                SEM_WRITECOUNT.decr();
                while (SEM_WRITECOUNT.getvalue() > 0)
                    ;
                SEM_NOOFWAITINGPROCESSES.incr();
                while (!SEM_NEXTPASS.getvalue())
                    ;
            }
            shmdt(arr);
            exit(0);
        }
    }
    
    
    
    SEM_BEGIN.set();
    
    for (int pass = 0; pass <= n - 1; pass++)
    {
        while (SEM_NOOFWAITINGPROCESSES.getvalue() < n / 2)
            ;
        SEM_PASSCOUNT.incr();
        while (SEM_NOOFWAITINGPROCESSES.getvalue() > 0)
            SEM_NOOFWAITINGPROCESSES.decr();
        while (SEM_READCOUNT.getvalue() < n / 2)
            SEM_READCOUNT.incr();
        while (SEM_WRITECOUNT.getvalue() < n / 2)
            SEM_WRITECOUNT.incr();
        while (SEM_EXECCOUNT.getvalue() < n / 2)
            SEM_EXECCOUNT.incr();
        SEM_NEXTPASS.set();
    }
    for (int i = 1; i <= n; i++)
    {
        cout << arr[i] << ' ';
    }
    cout << endl;

    shmdt(arr);

    shmctl(shmid_arr, IPC_RMID, NULL);
    return 0;
}
