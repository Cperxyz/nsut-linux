#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv)
{
    MPI_Init(NULL, NULL);
    int rnk;
    MPI_Comm_rank(MPI_COMM_WORLD, &rnk);
    int arr[8], selfNum, res;
    if(rnk == 0)
    {
        printf("Enter 8 elements\n");
        for(int i = 0; i < 8; i++)
        {
            scanf("%d", &arr[i]);
        }
        printf("You entered:\n");
        for(int i = 0; i < 8; i++)
        {
            printf("%d ", arr[i]);
        }
        printf("\n");
    }
    MPI_Scatter(arr, 1, MPI_INT, &selfNum, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Reduce(&selfNum, &res, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    if(rnk == 0)
    {
        printf("Their sum is: %d\n", res);
    }
    MPI_Finalize();
}
