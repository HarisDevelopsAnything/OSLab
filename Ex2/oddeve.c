#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>

int main()
{
    int fd[2];
    int data, pid;
    int s;
    s = pipe(fd);
    if (s < 0)
    {
        perror("Error when creating pipe!\n");
        exit(0);
    }
    pid = fork();
    if (pid > 0)
    {
        close(fd[0]);
        scanf("%d", &data);
        write(fd[1], &data, sizeof(data));
    }
    else
    {
        close(fd[1]);
        int num;
        read(fd[0], &num, 1);
        printf(num % 2 == 0 ? "Even\n" : "Odd\n");
    }
}