#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
int main()
{
	int pid;
	pid = fork();
	if (pid == 0)
	{
		int pid1;
		pid1 = fork();
		if (pid1 == 0)
		{
			printf("P3:\nProcess ID: %d\nParent ID: %d\n", getpid(), getppid());
		}
		else
		{
			printf("P2:\nProcess ID: %d\nParent ID: %d\n", getpid(), getppid());
		}
	}
	else
	{
		printf("P1:\nProcess ID: %d\nParent ID: %d\n", getpid(), getppid());
	}
}
