#include<iostream>
#include <sys/types.h>
#include <unistd.h>
#include <semaphore>

using namespace std;

class Semaphore
{
public:
    Semaphore()
    {
        
    }
    void incr()
    {}
    void decr()
    {}
    void set()
    {}
    void unset()
    {}
    int getValue()
    {}
};

#define endl '\n'

int main()
{
    int arr[8];
    cout<<"Enter 8 elements"<<endl;
    for(int i = 0; i < 8; i++)
    {
        cin>>arr[i];
    }
    int passCount = 0;
    bool odd = true;
    // Process 1
    if(!fork())
    {
        while(/*BEGIN is false*/);
        while(passCount < 8)
        {
            int i = 1, j = 2;
            //READ
            bool odd = odd;
            if(!odd)
            {
                i++, j++;
            }
            int tmp1 = arr[i];
            int tmp2 = arr[j];
            // READCOUNT--
            // wait while READCOUNT > 0
            //EXECUTE
            if(tmp1 > tmp2) swap(tmp1, tmp2);
            //SET NEXTPASS to false
            // EXECCOUNT--
            // wait while EXECCOUNT > 0
            //WRITE
            arr[i] = tmp1;
            arr[j] = tmp2;
            // WRITECOUNT--
            // wait while WRITECOUNT > 0
            // increment NoOfWaitingProcesses
            while(/*NEXTPASS is false*/);
        }
        exit(0);
    }
    // Process 2
    if(!fork())
    {
        while(/*BEGIN is false*/);
        while(passCount < 8)
        {
            int i = 3, j = 4;
            //READ
            bool odd = odd;
            if(!odd)
            {
                i++, j++;
            }
            int tmp1 = arr[i];
            int tmp2 = arr[j];
            // READCOUNT--
            // wait while READCOUNT > 0
            //EXECUTE
            if(tmp1 > tmp2) swap(tmp1, tmp2);
            //SET NEXTPASS to false
            // EXECCOUNT--
            // wait while EXECCOUNT > 0
            //WRITE
            arr[i] = tmp1;
            arr[j] = tmp2;
            // WRITECOUNT--
            // wait while WRITECOUNT > 0
            // increment NoOfWaitingProcesses
            while(/*NEXTPASS is false*/);
        }
        exit(0);
    }
    // Process 3
    if(!fork())
    {
        while(/*BEGIN is false*/);
        while(passCount < 8)
        {
            int i = 5, j = 6;
            //READ
            bool odd = odd;
            if(!odd)
            {
                i++, j++;
            }
            int tmp1 = arr[i];
            int tmp2 = arr[j];
            // READCOUNT--
            // wait while READCOUNT > 0
            //EXECUTE
            if(tmp1 > tmp2) swap(tmp1, tmp2);
            //SET NEXTPASS to false
            // EXECCOUNT--
            // wait while EXECCOUNT > 0
            //WRITE
            arr[i] = tmp1;
            arr[j] = tmp2;
            // WRITECOUNT--
            // wait while WRITECOUNT > 0
            // increment NoOfWaitingProcesses
            while(/*NEXTPASS is false*/);
        }
        exit(0);
    }
    // Process 4
    if(!fork())
    {
        while(/*BEGIN is false*/);
        while(passCount < 8)
        {
            int i = 7, j = 8;
            //READ
            bool odd = odd;
            if(!odd)
            {
                i++, j++;
            }
            int tmp1 = odd ? arr[i] : 0;
            int tmp2 = odd ? arr[j] : 0;
            // READCOUNT--
            // wait while READCOUNT > 0
            //EXECUTE
            if(tmp1 > tmp2) swap(tmp1, tmp2);
            //SET NEXTPASS to false
            // EXECCOUNT--
            // wait while EXECCOUNT > 0
            //WRITE            
            if(odd)
            {
                arr[i] = tmp1;
                arr[j] = tmp2;
            }
            // WRITECOUNT--
            // wait while WRITECOUNT > 0
            //increment NoOfWaitingProcesses
            while(/*NEXTPASS is false*/);
        }
        exit(0);
    }
    // Set begin semaphore to true
    for(int pass = 0; pass <= 9; pass++)
    {
        // wait while NoOfWaitingProcesses < 4
        odd = ((pass + 1) & 1) && true;
        passCount = pass;
        // set NoOfWaitingProcesses to 0
        // set READCOUNT to 4
        // set WRITECOUNT to 4
        // set EXECCOUNT to 4
        // set NEXTPASS to true
    }
    // display the arr
}
