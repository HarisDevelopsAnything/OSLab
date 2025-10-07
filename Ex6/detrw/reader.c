#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <unistd.h>
#include <time.h>

#define MUTEX 0
#define WRITE 1
#define SIZE 2 // [0]=data, [1]=read_count

void wait_sem(int semid, int semnum)
{
    struct sembuf sb = {semnum, -1, SEM_UNDO};
    semop(semid, &sb, 1);
}

void signal_sem(int semid, int semnum)
{
    struct sembuf sb = {semnum, +1, SEM_UNDO};
    semop(semid, &sb, 1);
}

void print_status(int *data, int reader_id, const char *action)
{
    time_t now = time(NULL);
    printf("[%ld] READER-%d: %s | Data=%d | Active_Readers=%d\n",
           now % 100, reader_id, action, data[0], data[1]);
    fflush(stdout);
}

int main()
{
    int shmid, semid;
    int *data;
    int reader_id = getpid() % 100; // Simple reader ID

    shmid = shmget(4000, SIZE * sizeof(int), 0666);
    if (shmid < 0)
    {
        perror("shmget");
        exit(1);
    }
    data = (int *)shmat(shmid, NULL, 0);

    semid = semget(5000, 2, 0666);
    if (semid < 0)
    {
        perror("semget");
        exit(1);
    }

    printf("=== READER-%d STARTED ===\n", reader_id);

    while (1)
    {
        // Entry section for reader
        printf("READER-%d: Requesting read access...\n", reader_id);
        wait_sem(semid, MUTEX);

        data[1]++; // read_count++
        printf("READER-%d: Read count increased to %d\n", reader_id, data[1]);

        if (data[1] == 1)
        {
            printf("READER-%d: First reader - blocking writers\n", reader_id);
            wait_sem(semid, WRITE); // First reader locks writers
        }

        signal_sem(semid, MUTEX);
        print_status(data, reader_id, "READING");

        // Critical section - simulate reading time
        int item = data[0];
        sleep(3); // Simulate reading time

        if (item == -1)
        {
            printf("READER-%d: Termination signal received\n", reader_id);
            // Exit section for reader
            wait_sem(semid, MUTEX);
            data[1]--;
            if (data[1] == 0)
            {
                printf("READER-%d: Last reader - unblocking writers\n", reader_id);
                signal_sem(semid, WRITE);
            }
            signal_sem(semid, MUTEX);
            printf("=== READER-%d TERMINATED ===\n", reader_id);
            break;
        }

        print_status(data, reader_id, "READ_COMPLETE");

        // Exit section for reader
        wait_sem(semid, MUTEX);
        data[1]--;
        printf("READER-%d: Read count decreased to %d\n", reader_id, data[1]);

        if (data[1] == 0)
        {
            printf("READER-%d: Last reader - unblocking writers\n", reader_id);
            signal_sem(semid, WRITE); // Last reader unlocks writers
        }
        signal_sem(semid, MUTEX);

        printf("READER-%d: Finished reading, waiting before next read...\n\n", reader_id);
        sleep(2);
    }

    shmdt(data);
    return 0;
}