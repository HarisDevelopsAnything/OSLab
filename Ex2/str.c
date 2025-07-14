#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>

int main()
{
    int fd[2];
    int fd1[2];
    char *data = (char *)malloc(sizeof(char) * 100);
    int pid;
    int s;
    s = pipe(fd);
    if (s < 0)
    {
        perror("Error when creating pipe!\n");
        exit(0);
    }
    s = pipe(fd1);
    if (s < 0)
    {
        perror("Error when creating second pipe!");
        exit(0);
    }
    pid = fork();
    if (pid > 0)
    {
        close(fd[0]);
        scanf("%s", data);
        write(fd[1], data, sizeof(data));
        write(fd1[1], data, sizeof(data));
        int pid1 = fork();
        if (pid1 == 0)
        {
            close(fd1[1]);
            char *rec = (char *)malloc(sizeof(char) * 100);
            read(fd1[0], rec, sizeof(char) * 100);
            int cnt = 0;
            char vows[10] = {'a', 'e', 'i', 'o', 'u', 'A', 'E', 'I', 'O', 'U'};
            for (int i = 0; i < strlen(rec); i++)
            {
                for (int j = 0; j < 10; j++)
                {
                    if (rec[i] == vows[j])
                    {
                        cnt++;
                        break;
                    }
                }
            }
            printf("No of vowels in string: %d\n", cnt);
        }
        else
        {
            close(fd1[0]);
        }
    }
    else
    {
        close(fd[1]);
        char *rec = (char *)malloc(sizeof(char) * 100);
        read(fd[0], rec, sizeof(char) * 100);
        for (int i = 0; i < strlen(rec) / 2; i++)
        {
            if (rec[i] != rec[strlen(rec) - i - 1])
            {
                printf("Not palindrome!\n");
                exit(0);
            }
        }
        printf("Palindrome!\n");
    }
}