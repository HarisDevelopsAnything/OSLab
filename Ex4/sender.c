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

    msgid = msgget(1104, IPC_CREAT | IPC_EXCL | 0777);
    if (msgid == -1)
    {
        perror("msgget failed");
        exit(1);
    }

    while (1)
    {
        printf("Enter a number (enter -1 to stop): ");
        scanf("%d", &item);
        msg.mtype = 1;
        msg.data = item;

        if (msgsnd(msgid, &msg, sizeof(msg.data), 0) == -1)
        {
            perror("msgsnd failed");
            exit(1);
        }

        if (item == -1)
            break;
    }

    return 0;
}