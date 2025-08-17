#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/types.h>

int main()
{
    int i, j, sid, c = 0;
    sid = shmget(1113, sizeof(int) * 49, IPC_CREAT | IPC_EXCL | 0666);
    if (sid == -1)
    {
        printf("Error when creating shared mem.\n");
    }
    int *arr = shmat(sid, 0, 0);
    arr[48] = 0;
    int A[4][4];
    int B[4][4];
    int C[4][4];
    int item;
    // get array a
    printf("Enter elements of arr A: \n");
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            printf("[%d][%d]: ", i, j);
            scanf("%d", &item);
            arr[c] = item;
            A[i][j] = item;
            c++;
            arr[48]++;
        }
    }
    // get array b
    printf("Enter the elements of array B: \n");
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            printf("[%d][%d]: ", i, j);
            scanf("%d", &item);
            B[i][j] = item;
            arr[c] = item;
            c++;
            arr[48]++;
        }
    }
    for (i = 0; i < 2; i++)
    {
        for (j = 0; j < 4; j++)
        {
            C[i][j] = 0;
            for (int k = 0; k < 4; k++)
            {
                C[i][j] += A[i][k] * B[k][j];
            }
            arr[c] = C[i][j];
            c++;
            arr[48]++;
        }
    }
    shmdt(arr);
    shmctl(sid, IPC_RMID, 0);
}