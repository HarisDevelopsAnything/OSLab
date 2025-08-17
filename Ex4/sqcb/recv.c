#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdlib.h>
#include <unistd.h>

struct message
{
    long mtype;
    int data;
};

int main()
{
    int msgid, item;
    struct message msg;

    msgid = msgget(1104, IPC_CREAT | 0777);
    if (msgid == -1)
    {
        perror("msgget failed - make sure producer is running first");
        exit(1);
    }

    while (1)
    {
        if (msgrcv(msgid, &msg, sizeof(msg.data), 0, 0) == -1)
        {
            perror("msgrcv failed");
            exit(1);
        }

        item = msg.data;

        if (item == -1)
        {
            printf("Consumer terminated\n");
            break;
        }
        if (msg.mtype == 1)
            printf("Received: %d and sqr= %d\n", item, item * item);
        else
            printf("Received %d and cube= %d\n", item, item * item * item);
    }

    if (msgctl(msgid, IPC_RMID, NULL) == 0)
        printf("Message queue removed successfully\n");
    else
        perror("Failed to remove message queue");
    return 0;
}