#include <stdio.h>
#include <stdlib.h>

int inPhy(int *phy, int pageNum, int ps)
{
    for (int i = 0; i < ps; i++)
    {
        if (phy[i] == pageNum)
            return 1;
    }
    return 0;
}

int checkRecent(int *refs, int ref, int r)
{
    int mr = -1;
    for (int i = 0; i < r; i++)
    {
        if (refs[i] == ref)
            mr = i;
    }
    return mr;
}

int checkLate(int *refs, int ref, int cr, int r)
{
    int ml = 999;
    for (int i = cr + 1; i < r; i++) 
    {
        if (refs[i] == ref)
        {
            ml = i;
            break; 
        }
    }
    return ml;
}

int lruRep(int *phyMem, int *refs, int r, int p, int cref)
{
    int lru = 0;
    int lr = 999;
    for (int i = 0; i < p; i++)
    {
        int cr = checkRecent(refs, phyMem[i], cref);
        if (cr < lr)
        {
            lru = i;
            lr = cr;
        }
    }
    return lru;
}

int optRep(int *phyMem, int *refs, int r, int p, int cref)
{
    int opt = 0;
    int l = -1;
    for (int i = 0; i < p; i++)
    {
        int cl = checkLate(refs, phyMem[i], cref, r);
        if (cl > l)
        {
            opt = i;
            l = cl;
        }
    }
    return opt;
}

void demPaging(int *phyMem, int *refs, int r, int p, int ch)
{
    int currRep = 0;
    int pfc = 0;
    for (int i = 0; i < r; i++)
    {
        if (inPhy(phyMem, refs[i], p) == 0)
        {
            int emptySlot = -1;
            for (int j = 0; j < p; j++)
            {
                if (phyMem[j] == -1)
                {
                    emptySlot = j;
                    break;
                }
            }

            if (emptySlot != -1)
            {
                phyMem[emptySlot] = refs[i];
                printf("Page fault for reference %d\n", refs[i]);
            }
            else
            {
                printf("Page fault for reference %d\n", refs[i]);
                if (ch == 1)
                {
                    currRep = lruRep(phyMem, refs, r, p, i);
                    printf("Page %d replaced with %d\n", phyMem[currRep], refs[i]);
                    phyMem[currRep] = refs[i];
                }
                else if (ch == 2)
                {
                    currRep = optRep(phyMem, refs, r, p, i);
                    printf("Page %d replaced with %d\n", phyMem[currRep], refs[i]);
                    phyMem[currRep] = refs[i];
                }
                else
                {
                    printf("Page %d replaced with %d\n", phyMem[currRep], refs[i]);
                    phyMem[currRep] = refs[i];
                    currRep = (currRep + 1) % p;
                }
            }
            pfc += 1;
        }
        else
        {
            printf("Page %d already in mem!\n", refs[i]);
        }

        printf("Memory state: [");
        for (int j = 0; j < p; j++)
        {
            if (phyMem[j] == -1)
                printf(" - ");
            else
                printf(" %d ", phyMem[j]);
        }
        printf("]\n\n");
    }
    printf("Total page faults: %d\n", pfc);
}

int main()
{
    printf("Enter the no. of references: ");
    int i, r, p;
    scanf("%d", &r);
    printf("Enter the physical memory size (pages): ");
    scanf("%d", &p);
    int phyMem[p];
    for (i = 0; i < p; i++)
    {
        phyMem[i] = -1;
    }
    int currRep = 0, pfc = 0;
    int refs[r];
    for (i = 0; i < r; i++)
    {
        printf("Enter the reference: ");
        scanf("%d", &refs[i]);
    }

    printf("\nChoose replacement algorithm:\n");
    printf("1. LRU\n");
    printf("2. Optimal\n");
    printf("3. FIFO\n");
    printf("Enter choice: ");
    int choice;
    scanf("%d", &choice);

    demPaging(phyMem, refs, r, p, choice);
    return 0; 
}