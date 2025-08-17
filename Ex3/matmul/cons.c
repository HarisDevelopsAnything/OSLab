#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/types.h>

int main()
{
    printf("Consumer\n");
    int i, j, sid, c = 0, in = 0;
    sid = shmget(1113, sizeof(int) * 49, IPC_CREAT | 0666);
    if (sid == -1)
    {
        printf("Error when creating shared mem.\n");
    }
    int *arr = shmat(sid, 0, 0);
    int A[4][4];
    int B[4][4];
    int C[4][4];
    int row = 0, col = 0;
    while (c < 48)
    {
        printf("Waiting...\n");
        while (arr[48] == 0)
            ;
        printf("Rec %d\n", arr[in]);
        c++;
        if (c < 16)
        {
            A[row][col] = arr[in];
            arr[48]--;
            in++;
            if (col < 3)
            {
                col++;
            }
            else
            {
                col = 0;
                row++;
            }
            if (row == 4)
            {
                row = 0;
            }
        }
        else if (c < 32)
        {
            B[row][col] = arr[in];
            in++;
            arr[48]--;
            if (row == 4)
            {
                row = 0;
            }
            if (col < 3)
            {
                col++;
            }
            else
            {
                col = 0;
                row++;
            }
        }
        else if (c < 40)
        {
            for (i = 0; i < 2; i++)
            {
                for (j = 0; j < 4; j++)
                {
                    C[i][j] = arr[in];
                    in++;
                }
            }
        }
        else
        {
            for (i = 2; i < 4; i++)
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
                }
            }
        }
    }
    printf("Got two arrays!\n");
    printf("Multiplication result: \n");
    for (i = 0; i < 4; i++)
    {
        for (j = 0; j < 4; j++)
        {
            printf("%d ", C[i][j]);
        }
        printf("\n");
    }
    shmdt(arr);
}