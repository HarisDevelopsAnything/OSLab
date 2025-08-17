#include <stdio.h>
#include <stdlib.h>

int main()
{
    printf("Enter the number of processes: ");
    int n;
    scanf("%d", &n);
    int bt[n], wt[n], tat[n];
    int total_wt = 0, total_tat = 0;
    printf("Enter the burst times of the processes:\n");
    for (int i = 0; i < n; i++)
    {
        printf("Process %d: ", i + 1);
        scanf("%d", &bt[i]);
    }
    printf("Enter the type of schediling:\n1. FCFS\n2. SRTF\n3. Round Robin\n4. Priority\n");
    int ch;
    scanf("%d", &ch);
    switch (ch)
    {
    case 1:        // FCFS
        wt[0] = 0; // First process has no waiting time
        for (int i = 1; i < n; i++)
        {
            wt[i] = wt[i - 1] + bt[i - 1];
        }
        for (int i = 0; i < n; i++)
        {
            tat[i] = bt[i] + wt[i];
            total_wt += wt[i];
            total_tat += tat[i];
        }
        printf("Process\tBT\tWT\tTAT\n");
        for (int i = 0; i < n; i++)
        {
            printf("%d\t%d\t%d\t%d\n", i + 1, bt[i], wt[i], tat[i]);
        }
        printf("Avg. Waiting Time: %.2f\n", (float)total_wt / n);
        printf("Avg. Turnaround Time: %.2f\n", (float)total_tat / n);
        break;
    case 2:
    { // srtf
        printf("Enter the arrival times of the processes:\n");
        int at[n];
        for (int i = 0; i < n; i++)
        {
            printf("Process %d: ", i + 1);
            scanf("%d", &at[i]);
        }
        int tot_bt = 0;
        for (int i = 0; i < n; i++)
        {
            tot_bt += bt[i];
        }
        int bt_copy[n];
        for (int i = 0; i < n; i++)
        {
            bt_copy[i] = bt[i]; // Copy burst times for SRTF
        }
        int curr_time = 0;
        while (curr_time < tot_bt)
        {
            int min_bt = 10000, min_index = -1;
            for (int i = 0; i < n; i++)
            {
                if (at[i] <= curr_time && bt[i] > 0 && bt[i] < min_bt)
                {
                    min_bt = bt[i];
                    min_index = i;
                }
            }
            if (min_index == -1) // No process is ready
            {
                curr_time++;
                continue;
            }
            printf("Process %d executed at time %d\n", min_index + 1, curr_time);
            bt[min_index]--;
            if (bt[min_index] == 0) // Process finished
            {
                tat[min_index] = curr_time + 1 - at[min_index];
                wt[min_index] = tat[min_index] - (bt[min_index] + 1);
                total_tat += tat[min_index];
                total_wt += wt[min_index];
            }
            curr_time++;
        }
        printf("All processes executed in SRTF scheduling.\n");
        printf("Process\tBT\tWT\tTAT\n");
        for (int i = 0; i < n; i++)
        {
            printf("%d\t%d\t%d\t%d\n", i + 1, bt_copy[i], wt[i], tat[i]);
        }
        printf("Avg. Waiting Time: %.2f\n", (float)total_wt / n);
        printf("Avg. Turnaround Time: %.2f\n", (float)total_tat / n);
        break;
    }
    case 3:
    {
        int tq;
        printf("Enter time quantum for Round Robin: ");
        scanf("%d", &tq);
        int cbt = 0;
        for (int i = 0; i < n; i++)
        {
            cbt += bt[i]; // Calculate total burst time
        }
        int cpn = 0;
        int stt[n];
        int ct[n];
        int bt_copy[n];
        for (int i = 0; i < n; i++)
        {
            bt_copy[i] = bt[i]; // Copy burst times for Round Robin
        }
        int curr_time = 0;
        while (cbt > 0)
        {
            if (bt[cpn] > 0)
            {
                if (bt[cpn] == bt_copy[cpn])
                {
                    printf("Process %d started at time %d\n", cpn + 1, curr_time);
                    stt[cpn] = curr_time; // Store start time for the process
                }
                if (bt[cpn] > tq)
                {
                    bt[cpn] -= tq;
                    cbt -= tq;
                    printf("Process %d executed for %d time units\n", cpn + 1, tq);
                    curr_time += tq;
                }
                else
                {
                    cbt -= bt[cpn];
                    curr_time += bt[cpn];
                    printf("Process %d executed for %d time units and finished at time %d\n", cpn + 1, bt[cpn], curr_time);
                    bt[cpn] = 0;
                    tat[cpn] = curr_time;
                    wt[cpn] = tat[cpn] - bt_copy[cpn];
                    total_tat += tat[cpn];
                    total_wt += wt[cpn];
                }
            }
            cpn = (cpn + 1) % n;
        }
        printf("All processes executed in Round Robin scheduling.\n");
        printf("Process\tBT\tWT\tTAT\n");
        for (int i = 0; i < n; i++)
        {
            printf("%d\t%d\t%d\t%d\n", i + 1, bt_copy[i], wt[i], tat[i]);
        }
        printf("Avg. Waiting Time: %.2f\n", (float)total_wt / n);
        printf("Avg. Turnaround Time: %.2f\n", (float)total_tat / n);
        break;
    }
    case 4:
    { // Priority
        int pr[n];
        for (int i = 0; i < n; i++)
        {
            printf("Enter priority for process %d: ", i + 1);
            scanf("%d", &pr[i]);
        }
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n - i - 1; j++)
            {
                if (pr[j] > pr[j + 1])
                { // Sort processes based on priority
                    int temp = pr[j];
                    pr[j] = pr[j + 1];
                    pr[j + 1] = temp;
                    temp = bt[j];
                    bt[j] = bt[j + 1];
                    bt[j + 1] = temp;
                }
            }
        }
        for (int i = 0; i < n; i++)
        {
            wt[i] = (i == 0) ? 0 : wt[i - 1] + bt[i - 1];
            tat[i] = bt[i] + wt[i];
            total_wt += wt[i];
            total_tat += tat[i];
        }
        printf("Processes sorted by priority:\n");
        printf("Process\tBT\tWT\tPriority\n");
        for (int i = 0; i < n; i++)
        {
            printf("%d\t%d\t%d\t%d\n", i + 1, bt[i], wt[i], pr[i]);
        }
        printf("Avg. Waiting Time: %.2f\n", (float)total_wt / n);
        printf("Avg. Turnaround Time: %.2f\n", (float)total_tat / n);
        break;
    }
    default:
        printf("Invalid choice!\n");
    }
}