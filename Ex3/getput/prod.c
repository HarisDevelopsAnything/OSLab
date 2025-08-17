#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/types.h>

int main()
{
    int i, j, sid, item;
    int in = 0, n = 5;
    int count = 0;
    sid = shmget(1109, sizeof(int) * 5, IPC_CREAT | IPC_EXCL | 0666); // 4 read 2 write 1 execute
    if (sid == -1)
    {
        printf("Error when creating shared mem.\n");
    }
    int *data = shmat(sid, 0, 0);
    data[n] = 0;
    while (count < 5)
    {
        scanf("%d", &item);
        while (data[n] == n)
            ;
        data[in] = item;
        in = (in + 1) % n;
        data[n]++;
        count++;
    }
    shmdt(data);
    shmctl(sid, IPC_RMID, 0);
}