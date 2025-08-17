#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/types.h>

int main()
{
    int i, j, sid, item;
    int out = 0, n = 5, count = 0;
    sid = shmget(1109, sizeof(int) * 5, IPC_CREAT | 0666);

    if (sid == -1)
    {
        printf("Error when creating shared mem.\n");
    }
    int *data = shmat(sid, 0, 0);
    while (count < 5)
    {
        while (data[n] == 0)
            ;
        item = data[out];
        printf("%d\n", item);
        out = (out + 1) % n;
        data[n]--;
        count++;
    }
    shmdt(data);
}