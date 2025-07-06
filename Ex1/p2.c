#include <stdio.h>
#include <unistd.h>

int main()
{
	int pid;
	printf("P1:\nProcess ID: %d\nParent ID: %d\n", getpid(), getppid());
	pid = fork();
	if (pid == 0)
	{
		printf("P2:\nProcess ID: %d\nParent ID: %d\n", getpid(), getppid());
	}
	else
	{
		int pid1;
		pid1 = fork();
		if (pid1 == 0)
		{
			printf("P3:\nProcess ID: %d\nParent ID: %d\n", getpid(), getppid());
		}
	}
}
