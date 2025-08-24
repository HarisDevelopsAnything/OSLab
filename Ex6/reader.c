#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <unistd.h>

#define MUTEX 0
#define WRITE 1
#define SIZE 2 // [0]=data, [1]=read_count

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

    while (1) {
        // Entry section for reader
        wait_sem(semid, MUTEX);
        data[1]++; // read_count++
        if (data[1] == 1) {
            wait_sem(semid, WRITE); // First reader locks writers
        }
        signal_sem(semid, MUTEX);

        // Critical section
        int item = data[0];
        if (item == -1) {
            printf("Reader terminated\n");
            // Exit section for reader
            wait_sem(semid, MUTEX);
            data[1]--;
            if (data[1] == 0) {
                signal_sem(semid, WRITE);
            }
            signal_sem(semid, MUTEX);
            break;
        }
        printf("Reader read: %d\n", item);

        // Exit section for reader
        wait_sem(semid, MUTEX);
        data[1]--;
        if (data[1] == 0) {
            signal_sem(semid, WRITE); // Last reader unlocks writers
        }
        signal_sem(semid, MUTEX);

        sleep(1);
    }

    shmdt(data);
    return 0;
}