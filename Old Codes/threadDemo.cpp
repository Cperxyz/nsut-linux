#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

bool oddPass;
int pass;

void* threadFn(void* args)
{
    if(oddPass)
    {
        printf("Thread: %ld --> Pass %d: Odd\n", pthread_self(), pass);
    }
    else
    {
        printf("Thread: %ld --> Pass %d: Even\n", pthread_self(), pass);
    }
    return NULL;
}

int main()
{
    int n;
    printf("Enter n\n");
    scanf("%d", &n);
    pthread_t *t_ids = (pthread_t*)malloc((n + 1) * sizeof(pthread_t));
    for(int i = 1; i <= n; i++)
    {
        bool odd = i & 1;
        pass = i;
        oddPass = odd;
        for(int i = 0; i < n / 2; i++)
        {
            pthread_create(&t_ids[i], NULL, threadFn, NULL);
        }
        for(int i = 0; i < n / 2; i++)
        {
            pthread_join(t_ids[i], NULL);
        }
    }
    printf("main ends\n");
    return 0;
}
