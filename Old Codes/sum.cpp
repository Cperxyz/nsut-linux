#include <iostream>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

using namespace std;

#define endl '\n'

int main()
{
    int status = 0;
    unsigned int n;
    cout<<"Enter the number of numbers to add (a power of 2)"<<endl;
    cin>>n;
    key_t key = ftok("shmfile", 65);
    int shmid = shmget(key, (n + 3) * sizeof(int), IPC_CREAT | 0666);
    int* arr = (int*)shmat(shmid, NULL, 0);
    cout<<"Enter the array elements"<<endl;
    for(int i = 1; i <= n; i++)
    {
        cin>>arr[i];
    }
    int p = n / 2;
    while(p > 0)
    {
        for(int i = 1; i <= p; i++)
        {
            if(!fork())
            {
                int* arr = (int*)shmat(shmid, NULL, 0);
                
                //READ
                int elt1 = arr[2*i - 1];
                int elt2 = arr[2*i];
                
                //COMPUTE
                int sum = elt1 + elt2;
                
                //WRITE
                arr[i] = sum;
                
                shmdt(arr);
                exit(0);
            }
        }
        while(wait(&status) > 0);
        p = p / 2;
    }
    cout<<"Sum is "<<arr[1]<<endl;
    shmdt(arr);
    shmctl(shmid, IPC_RMID, NULL);
    return 0;
}
