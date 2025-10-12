#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void sort(int *arr, int n)
{
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n - i - 1; j++)
        {
            if (arr[j] > arr[j + 1])
            {
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

void FCFS(int *cylst, int reqcnt, int totcy, int inithp)
{
    int total = 0;
    int pre = inithp;
    for (int i = 0; i < reqcnt; i++)
    {
        total += abs(cylst[i] - pre);
        printf("%d to %d: %d\n", pre, cylst[i], abs(cylst[i] - pre));
        pre = cylst[i];
    }
    printf("Total distance: %d\n", total);
}

void SSTF(int *cylst, int reqcnt, int totcy, int inithp)
{
    int total = 0;
    int pre = inithp;
    int *temp = (int *)malloc(sizeof(int) * reqcnt);
    for (int i = 0; i < reqcnt; i++)
        temp[i] = cylst[i];
    sort(temp, reqcnt);

    int *finished = (int *)calloc(sizeof(int), reqcnt);
    int finc = 0;

    while (finc < reqcnt)
    {
        int min = 999999;
        int curr = -1;
        for (int i = 0; i < reqcnt; i++)
        {
            if (finished[i] == 0 && abs(temp[i] - pre) < min)
            {
                min = abs(temp[i] - pre);
                curr = i;
            }
        }
        printf("%d to %d: %d\n", pre, temp[curr], abs(temp[curr] - pre));
        total += abs(temp[curr] - pre);
        pre = temp[curr];
        finished[curr] = 1;
        finc++;
    }
    printf("Total distance: %d\n", total);
    free(temp);
    free(finished);
}

void SCAN(int *cylst, int reqcnt, int totcy, int inithp)
{
    int total = 0;
    int pre = inithp;
    int *temp = (int *)malloc(sizeof(int) * reqcnt);
    for (int i = 0; i < reqcnt; i++)
        temp[i] = cylst[i];
    sort(temp, reqcnt);

    int curr = 0;
    for (int i = 0; i < reqcnt; i++)
    {
        if (temp[i] >= inithp)
        {
            curr = i;
            break;
        }
    }

    for (int i = curr; i < reqcnt; i++)
    {
        printf("%d to %d: %d\n", pre, temp[i], abs(temp[i] - pre));
        total += abs(temp[i] - pre);
        pre = temp[i];
    }

    if (pre != totcy - 1)
    {
        printf("%d to %d: %d\n", pre, totcy - 1, totcy - 1 - pre);
        total += totcy - 1 - pre;
        pre = totcy - 1;
    }

    for (int i = curr - 1; i >= 0; i--)
    {
        printf("%d to %d: %d\n", pre, temp[i], abs(temp[i] - pre));
        total += abs(temp[i] - pre);
        pre = temp[i];
    }

    printf("Total distance: %d\n", total);
    free(temp);
}

void CSCAN(int *cylst, int reqcnt, int totcy, int inithp)
{
    int total = 0;
    int pre = inithp;
    int *temp = (int *)malloc(sizeof(int) * reqcnt);
    for (int i = 0; i < reqcnt; i++)
        temp[i] = cylst[i];
    sort(temp, reqcnt);

    int curr = 0;
    for (int i = 0; i < reqcnt; i++)
    {
        if (temp[i] >= inithp)
        {
            curr = i;
            break;
        }
    }

    for (int i = curr; i < reqcnt; i++)
    {
        printf("%d to %d: %d\n", pre, temp[i], abs(temp[i] - pre));
        total += abs(temp[i] - pre);
        pre = temp[i];
    }

    if (pre != totcy - 1)
    {
        printf("%d to %d: %d\n", pre, totcy - 1, totcy - 1 - pre);
        total += totcy - 1 - pre;
        pre = totcy - 1;
    }

    printf("%d to %d: %d\n", pre, 0, pre);
    total += pre;
    pre = 0;

    for (int i = 0; i < curr; i++)
    {
        printf("%d to %d: %d\n", pre, temp[i], abs(temp[i] - pre));
        total += abs(temp[i] - pre);
        pre = temp[i];
    }

    printf("Total distance: %d\n", total);
    free(temp);
}

void LOOK(int *cylst, int reqcnt, int totcy, int inithp)
{
    int total = 0;
    int pre = inithp;
    int *temp = (int *)malloc(sizeof(int) * reqcnt);
    for (int i = 0; i < reqcnt; i++)
        temp[i] = cylst[i];
    sort(temp, reqcnt);

    int curr = 0;
    for (int i = 0; i < reqcnt; i++)
    {
        if (temp[i] >= inithp)
        {
            curr = i;
            break;
        }
    }

    for (int i = curr; i < reqcnt; i++)
    {
        printf("%d to %d: %d\n", pre, temp[i], abs(temp[i] - pre));
        total += abs(temp[i] - pre);
        pre = temp[i];
    }

    for (int i = curr - 1; i >= 0; i--)
    {
        printf("%d to %d: %d\n", pre, temp[i], abs(temp[i] - pre));
        total += abs(temp[i] - pre);
        pre = temp[i];
    }

    printf("Total distance: %d\n", total);
    free(temp);
}

void CLOOK(int *cylst, int reqcnt, int totcy, int inithp)
{
    int total = 0;
    int pre = inithp;
    int *temp = (int *)malloc(sizeof(int) * reqcnt);
    for (int i = 0; i < reqcnt; i++)
        temp[i] = cylst[i];
    sort(temp, reqcnt);

    int curr = 0;
    for (int i = 0; i < reqcnt; i++)
    {
        if (temp[i] >= inithp)
        {
            curr = i;
            break;
        }
    }

    for (int i = curr; i < reqcnt; i++)
    {
        printf("%d to %d: %d\n", pre, temp[i], abs(temp[i] - pre));
        total += abs(temp[i] - pre);
        pre = temp[i];
    }

    if (curr > 0)
    {
        printf("%d to %d: %d\n", pre, temp[0], abs(temp[0] - pre));
        total += abs(temp[0] - pre);
        pre = temp[0];
    }

    for (int i = 1; i < curr; i++)
    {
        printf("%d to %d: %d\n", pre, temp[i], abs(temp[i] - pre));
        total += abs(temp[i] - pre);
        pre = temp[i];
    }

    printf("Total distance: %d\n", total);
    free(temp);
}

int main()
{
    int na, nc;
    printf("Enter the number of accesses: ");
    scanf("%d", &na);
    printf("Enter the number of cylinders: ");
    scanf("%d", &nc);
    int acc[na];
    printf("Enter the accesses: ");
    for (int i = 0; i < na; i++)
    {
        scanf("%d", &acc[i]);
    }
    int ch;
    int hp;
    while (1)
    {
        printf("1) FCFS 2) SSTF 3) SCAN 4) C-SCAN 5) LOOK 6) C-LOOK 7) Exit\n");
        scanf("%d", &ch);
        printf("Enter the head position: ");
        scanf("%d", &hp);
        switch (ch)
        {
        case 1:
        {
            int total = 0;
            int pre = hp;
            for (int i = 0; i < na; i++)
            {
                total += abs(acc[i] - pre);
                printf("%d to %d: %d\n", pre, acc[i], abs(acc[i] - pre));
                pre = acc[i];
            }
            printf("Total distance: %d\n", total);
            break;
        }
        case 2:
        {
            SSTF(acc, na, nc, hp);
            break;
        }
        case 3:
        {
            SCAN(acc, na, nc, hp);
            break;
        }
        case 4:
        {
            CSCAN(acc, na, nc, hp);
            break;
        }
        case 5:
        {
            LOOK(acc, na, nc, hp);
            break;
        }
        case 6:
        {
            CLOOK(acc, na, nc, hp);
            break;
        }
        case 7:
        {
            printf("Exiting.\n");
            return 0;
        }
        default:
            printf("Invalid choice!\n");
        }
    }
}