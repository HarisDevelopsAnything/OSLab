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
    int msgid, item, t;
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
        if (item != -1)
        {
            while (1)
            {
                printf("Enter the type: 1 for square, 2 for cube: ");
                scanf("%d", &t);
                if (t == 1 || t == 2)
                    break;
                printf("Invalid choice!\n");
            }
        }
        msg.mtype = t;
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