#include<iostream>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <vector>

using namespace std;

#define endl '\n'

int main()
{
    int status = 0;
    int n;
    cout<<"Enter the number of elements (even)"<<endl;
    cin>>n;
    key_t key = ftok("shmfile2", 65);
    int shmid = shmget(key, (n + 3) * sizeof(int), IPC_CREAT | 0666);
    int* arr = (int*)shmat(shmid, NULL, 0);
    cout<<"Enter the elements."<<endl;
    for(int i = 1; i <= n; i++)
    {
        cin>>arr[i];
    }
    for(int pass = 1; pass <= n; pass++)
    {
        if((pass % 2) == 1)
        {
            for(int i = 1; i < n; i += 2)
            {
                if(!fork())
                {
                    int* arr = (int*)shmat(shmid, NULL, 0);
                    
                    //READ
                    int e1 = arr[i], e2 = arr[i+1];
                    
                    //COMPUTE
                    if(e1 > e2) swap(e1, e2);
                    
                    //WRITE
                    arr[i] = e1;
                    arr[i+1] = e2;
                    
                    shmdt(arr);
                    exit(0);
                }
            }
            while(wait(&status) > 0);
        }
        else
        {
            for(int i = 2; i < n - 1; i += 2)
            {
                if(!fork())
                {
                    int* arr = (int*)shmat(shmid, NULL, 0);
                    
                    //READ
                    int e1 = arr[i], e2 = arr[i+1];
                    
                    //COMPUTE
                    if(e1 > e2) swap(e1, e2);
                    
                    //WRITE
                    arr[i] = e1;
                    arr[i+1] = e2;
                    
                    shmdt(arr);
                    exit(0);
                }
            }
            while(wait(&status) > 0);
        }
    }
    cout<<"The sorted array is"<<endl;
    for(int i = 1; i <= n; i++) cout<<arr[i]<<' '; cout<<endl;
    shmdt(arr);
    shmctl(shmid, IPC_RMID, NULL);
    return 0;
}
