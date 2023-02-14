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

int log2(unsigned int n)
{
    for(int i = 30; i >= 0; i--)
    {
        if(n & (1 << i)) return i;
    }
    return -1;
}

int main()
{
    int n = 16;
    key_t key = ftok("shmfile1", 65);
    int shmid = shmget(key, (n + 3) * sizeof(int), IPC_CREAT | 0666);
    int* arr = (int*)shmat(shmid, NULL, 0);
    cout<<"Enter 16 elements to add."<<endl;
    for(int i = 1; i <= n; i++)
    {
        cin>>arr[i];
    }
    int status = 0;
    while(n > 2)
    {
        int l = log2(n);
        int ng = n / l;
        for(int ll = 1, i = 1; i <= ng; i++, ll += l)
        {
            int ul = ll + l - 1;
            if(!fork())
            {
                int* arr = (int*)shmat(shmid, NULL, 0);
                int sum = 0;
                vector<int> curElts;
                
                // READ
                for(int j = ll; j <= ul; j++)
                {
                    curElts.push_back(arr[j]);
                }
                
                //COMPUTE
                for(auto itr: curElts) sum += itr;
                
                //WRITE
                arr[i] = sum;
                
                shmdt(arr);
                exit(0);
            }
        }
        while(wait(&status) > 0);
        n = ng;
    }
    int res = arr[1] + arr[2];
    cout<<"Sum is: "<<res<<endl;
    shmdt(arr);
    shmctl(shmid, IPC_RMID, NULL);
    return 0;
}
