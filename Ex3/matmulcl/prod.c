#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/types.h>

int main()
{
    printf("Producer\n");
    int i, j, sid, c = 0;
    sid = shmget(1113, sizeof(int) * 49, IPC_CREAT | IPC_EXCL | 0666);
    if (sid == -1)
    {
        printf("Error when creating shared mem.\n");
        exit(1);
    }

    int *arr = shmat(sid, 0, 0);
    arr[48] = 0; // Counter for available items

    int A[4][4];
    int B[4][4];
    int C[4][4];
    int item;

    // Get matrix A
    printf("Enter elements of matrix A: \n");
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            printf("A[%d][%d]: ", i, j);
            scanf("%d", &item);
            arr[c] = item;
            A[i][j] = item;
            c++;
            arr[48]++;
        }
    }

    // Get matrix B
    printf("Enter elements of matrix B (4x4): \n");
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            printf("B[%d][%d]: ", i, j);
            scanf("%d", &item);
            B[i][j] = item;
            arr[c] = item;
            c++;
            arr[48]++;
        }
    }

    // Calculate first 2 rows of C = A * B
    printf("Producer- calculating first 2 rows\n");
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
            printf("C[%d][%d] = %d\n", i, j, C[i][j]);
        }
    }
    shmdt(arr);
    return 0;
}
