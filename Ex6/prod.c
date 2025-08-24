#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>

#define MUTEX 0
#define EMPTY 1
#define FULL 2
#define SIZE 5

union semun
{
    int val;
    struct semid_ds *buf;
    unsigned short *array;
};

void wait_sem(int semid, int semnum)
{
    struct sembuf sb;
    sb.sem_num = semnum;
    sb.sem_op = -1;
    sb.sem_flg = SEM_UNDO;
    semop(semid, &sb, 1);
}

void signal_sem(int semid, int semnum)
{
    struct sembuf sb;
    sb.sem_num = semnum;
    sb.sem_op = +1;
    sb.sem_flg = SEM_UNDO;
    semop(semid, &sb, 1);
}

int main()
{
    int shmid, semid;
    int *data;
    int in = 0, item;

    shmid = shmget(2000, SIZE * sizeof(int), IPC_CREAT | IPC_EXCL | 0777);
    if (shmid < 0)
    {
        perror("shmget");
        exit(1);
    }
    data = (int *)shmat(shmid, NULL, 0);

    semid = semget(3000, 3, IPC_CREAT | IPC_EXCL | 0777);
    if (semid < 0)
    {
        perror("semget");
        exit(1);
    }

    union semun u;
    unsigned short values[3] = {1, SIZE, 0};
    u.array = values;
    semctl(semid, 0, SETALL, u);

    while (1)
    {
        printf("Enter a number (-1 to stop): ");
        scanf("%d", &item);

        wait_sem(semid, EMPTY);
        wait_sem(semid, MUTEX);

        data[in] = item;
        in = (in + 1) % SIZE;

        signal_sem(semid, MUTEX);
        signal_sem(semid, FULL);

        if (item == -1)
            break;
    }

    shmdt(data);
    return 0;
}