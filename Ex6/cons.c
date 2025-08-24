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
    int out = 0, item;

    shmid = shmget(2000, SIZE * sizeof(int), IPC_CREAT | 0666);
    if (shmid < 0)
    {
        perror("shmget");
        exit(1);
    }
    data = (int *)shmat(shmid, NULL, 0);

    semid = semget(3000, 3, IPC_CREAT | 0666);
    if (semid < 0)
    {
        perror("semget");
        exit(1);
    }

    while (1)
    {
        wait_sem(semid, FULL);
        wait_sem(semid, MUTEX);

        item = data[out];
        out = (out + 1) % SIZE;

        signal_sem(semid, MUTEX);
        signal_sem(semid, EMPTY);

        if (item == -1)
        {
            printf("Consumer terminated\n");
            break;
        }
        printf("Consumed: %d\n", item);
    }

    shmdt(data);
    shmctl(shmid, IPC_RMID, 0);
    semctl(semid, 0, IPC_RMID);
    return 0;
}