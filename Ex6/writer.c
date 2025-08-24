#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>

#define MUTEX 0
#define WRITE 1
#define SIZE 1

union semun {
    int val;
    struct semid_ds *buf;
    unsigned short *array;
};

void wait_sem(int semid, int semnum) {
    struct sembuf sb = {semnum, -1, SEM_UNDO};
    semop(semid, &sb, 1);
}

void signal_sem(int semid, int semnum) {
    struct sembuf sb = {semnum, +1, SEM_UNDO};
    semop(semid, &sb, 1);
}

int main() {
    int shmid, semid;
    int *data;

    shmid = shmget(4000, SIZE * sizeof(int), IPC_CREAT | 0666);
    if (shmid < 0) { perror("shmget"); exit(1); }
    data = (int *)shmat(shmid, NULL, 0);

    semid = semget(5000, 2, IPC_CREAT | 0666);
    if (semid < 0) { perror("semget"); exit(1); }

    union semun u;
    unsigned short values[2] = {1, 1}; // MUTEX=1, WRITE=1
    u.array = values;
    semctl(semid, 0, SETALL, u);

    while (1) {
        int item;
        printf("Writer: Enter a number (-1 to stop): ");
        scanf("%d", &item);

        wait_sem(semid, WRITE);
        wait_sem(semid, MUTEX);

        data[0] = item;

        signal_sem(semid, MUTEX);
        signal_sem(semid, WRITE);

        if (item == -1) break;
    }

    shmdt(data);
    shmctl(shmid, IPC_RMID, 0);
    semctl(semid, 0, IPC_RMID);
    return 0;
}