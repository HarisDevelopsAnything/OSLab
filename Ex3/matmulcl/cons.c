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
    sid = shmget(1113, sizeof(int) * 49, 0666);
    if (sid == -1)
    {
        printf("Error getting shared memory.\n");
        exit(1);
    }

    int *arr = shmat(sid, 0, 0);
    int A[4][4];
    int B[4][4];
    int C[4][4];
    int row = 0, col = 0;

    printf("Input matrix A:\n");
    while (c < 16)
    {
        while (arr[48] <= in)
            ;

        A[row][col] = arr[in];
        printf("A[%d][%d] = %d\n", row, col, A[row][col]);

        in++;
        c++;

        col++;
        if (col == 4)
        {
            col = 0;
            row++;
        }
    }

    row = 0;
    col = 0;

    printf("Input matrix B:\n");
    while (c < 32)
    {
        while (arr[48] <= in)
            ;

        B[row][col] = arr[in];
        printf("B[%d][%d] = %d\n", row, col, B[row][col]);

        in++;
        c++;

        col++;
        if (col == 4)
        {
            col = 0;
            row++;
        }
    }
    printf("Get first 2 rows in result from producer...\n");
    for (i = 0; i < 2; i++)
    {
        for (j = 0; j < 4; j++)
        {
            while (arr[48] <= in)
                ; // Wait for data

            C[i][j] = arr[in];
            printf("C[%d][%d] = %d (from producer)\n", i, j, C[i][j]);

            in++;
            c++;
        }
    }

    // Calculate last 2 rows of C
    printf("Cons. calculates last 2 rows of C...\n");
    for (i = 2; i < 4; i++)
    {
        for (j = 0; j < 4; j++)
        {
            C[i][j] = 0;
            for (int k = 0; k < 4; k++)
            {
                C[i][j] += A[i][k] * B[k][j];
            }
            printf("C[%d][%d] = %d\n", i, j, C[i][j]);
        }
    }

    // Print final result
    printf("\nMultiplication result:\n");

    for (i = 0; i < 4; i++)
    {
        for (j = 0; j < 4; j++)
        {
            printf("%d ", C[i][j]);
        }
        printf("\n");
    }

    shmdt(arr);
    shmctl(sid, IPC_RMID, 0);
    return 0;
}
