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

union semun
{
    int val;
    struct semid_ds *buf;
    unsigned short *array;
};

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

void print_status(int *data, int writer_id, const char *action)
{
    time_t now = time(NULL);
    printf("[%ld] WRITER-%d: %s | Data=%d | Active_Readers=%d\n",
           now % 100, writer_id, action, data[0], data[1]);
    fflush(stdout);
}

int main()
{
    int shmid, semid;
    int *data;
    int writer_id = getpid() % 100; // Simple writer ID

    shmid = shmget(4000, SIZE * sizeof(int), IPC_CREAT | 0666);
    if (shmid < 0)
    {
        perror("shmget");
        exit(1);
    }
    data = (int *)shmat(shmid, NULL, 0);

    semid = semget(5000, 2, IPC_CREAT | 0666);
    if (semid < 0)
    {
        perror("semget");
        exit(1);
    }

    // Initialize semaphores
    union semun u;
    unsigned short values[2] = {1, 1}; // MUTEX=1, WRITE=1
    u.array = values;
    semctl(semid, 0, SETALL, u);

    // Initialize shared data
    data[0] = 0; // initial data
    data[1] = 0; // read_count

    printf("=== WRITER-%d STARTED ===\n", writer_id);

    while (1)
    {
        int item;
        printf("Writer-%d: Enter a number (-1 to stop): ", writer_id);
        scanf("%d", &item);

        printf("WRITER-%d: Requesting write access...\n", writer_id);
        print_status(data, writer_id, "WAITING_FOR_WRITE");

        wait_sem(semid, WRITE); // Wait for write permission
        printf("WRITER-%d: Got write lock, waiting for mutex...\n", writer_id);

        wait_sem(semid, MUTEX); // Get exclusive access
        printf("WRITER-%d: WRITING DATA...\n", writer_id);
        print_status(data, writer_id, "WRITING");

        // Simulate writing time
        sleep(2);
        data[0] = item;

        print_status(data, writer_id, "WRITE_COMPLETE");
        signal_sem(semid, MUTEX);
        signal_sem(semid, WRITE);

        printf("WRITER-%d: Released write access\n\n", writer_id);

        if (item == -1)
        {
            printf("=== WRITER-%d TERMINATING ===\n", writer_id);
            break;
        }
    }

    shmdt(data);
    return 0;
}