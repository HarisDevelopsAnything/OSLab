#include <stdio.h>
#include <unistd.h>

int main()
{
    int sum = 0;
    int n;
    printf("Enter the no. of elements: ");
    scanf("%d", &n);
    int arr[n];
    for (int i = 0; i < n; i++)
    {
        scanf("%d", &arr[i]);
    }
    int pid = fork();
    if (pid == 0)
    {
        for (int i = 0; i < n / 2; i++)
        {
            sum += arr[i];
        }
        printf("Sum of first half= %d\n", sum);
    }
    else
    {
        for (int i = n / 2; i < n; i++)
        {
            sum += arr[i];
        }
        printf("Sum of sec. half= %d\n", sum);
    }
}