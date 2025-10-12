#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#define blocks 100

struct directory
{
    int startblock;
    char *dirname;
    int size;
} dirs[100];

static int dirc = -1;

int allocateFile(int *freelst, int freeCount, int blocksize, int filesize, char *name)
{
    int nb = ceil((double)filesize / blocksize);

    // Count total free blocks
    int totalFree = 0;
    for (int i = 0; i < blocks; i++)
    {
        if (freelst[i] == 1)
            totalFree++;
    }

    if (nb > totalFree)
    {
        printf("Not enough blocks! Need %d blocks, only %d free.\n", nb, totalFree);
        return -1;
    }

    // Find contiguous free blocks
    int cont = 0;
    int start = -1;

    for (int i = 0; i < blocks; i++)
    {
        if (freelst[i] == 1)
        {
            if (cont == 0)
                start = i;
            cont++;

            if (cont == nb)
            {
                // Allocate blocks
                for (int j = start; j < start + nb; j++)
                {
                    freelst[j] = 0;
                }

                // Add to directory
                struct directory newdir;
                newdir.startblock = start;
                newdir.dirname = name;
                newdir.size = filesize;
                dirc++;
                dirs[dirc] = newdir;

                printf("File '%s' allocated successfully!\n", name);
                return start;
            }
        }
        else
        {
            cont = 0;
            start = -1;
        }
    }

    printf("Could not find %d contiguous free blocks!\n", nb);
    return -1;
}

int deallocateFile(int *freelst, char *filename, int blocksize)
{
    // Find file in directory
    int fileIndex = -1;
    for (int i = 0; i <= dirc; i++)
    {
        if (strcmp(dirs[i].dirname, filename) == 0)
        {
            fileIndex = i;
            break;
        }
    }

    if (fileIndex == -1)
    {
        printf("File not found!\n");
        return -1;
    }

    int start = dirs[fileIndex].startblock;
    int filesize = dirs[fileIndex].size;
    int nb = ceil((double)filesize / blocksize);

    // Free the blocks
    for (int i = start; i < start + nb; i++)
    {
        freelst[i] = 1;
    }

    // Remove from directory by shifting
    for (int i = fileIndex; i < dirc; i++)
    {
        dirs[i] = dirs[i + 1];
    }
    dirc--;

    printf("File '%s' deallocated successfully!\n", filename);
    return 0;
}

void showFreeBlocks(int *freelst)
{
    printf("Free blocks: ");
    int count = 0;
    for (int i = 0; i < blocks; i++)
    {
        if (freelst[i] == 1)
        {
            printf("%d ", i);
            count++;
        }
    }
    printf("\nTotal free blocks: %d\n", count);
}

void showDirectories(int blocksize)
{
    if (dirc == -1)
    {
        printf("No files allocated\n");
        return;
    }

    printf("\n--- File Directory ---\n");
    for (int i = 0; i <= dirc; i++)
    {
        printf("File name: %s\n", dirs[i].dirname);
        printf("File size: %d bytes\n", dirs[i].size);
        int nb = ceil((double)dirs[i].size / blocksize);
        printf("Blocks occupied (%d): ", nb);
        for (int j = dirs[i].startblock; j < dirs[i].startblock + nb; j++)
            printf("%d ", j);
        printf("\n\n");
    }
}

int main()
{
    int freelst[blocks];
    int blocksize, freeCount = 0;

    // Initialize all blocks as allocated
    for (int i = 0; i < blocks; i++)
        freelst[i] = 0;

    // Read free blocks from file
    FILE *fp = fopen("freeblocks.txt", "r");
    if (fp == NULL)
    {
        printf("Error opening file! Creating default free blocks...\n");
        // Default: make all blocks free
        for (int i = 0; i < blocks; i++)
            freelst[i] = 1;
        freeCount = blocks;
    }
    else
    {
        int block;
        while (fscanf(fp, "%d", &block) == 1)
        {
            if (block >= 0 && block < blocks)
            {
                freelst[block] = 1;
                freeCount++;
            }
        }
        fclose(fp);
        printf("Loaded %d free blocks from file\n", freeCount);
    }

    printf("Enter block size (in bytes): ");
    scanf("%d", &blocksize);

    int choice;
    do
    {
        printf("\n========== MENU ==========\n");
        printf("1. Allocate file\n");
        printf("2. Deallocate file\n");
        printf("3. Show free blocks\n");
        printf("4. Show directories\n");
        printf("5. Exit\n");
        printf("==========================\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
        {
            char *filename = (char *)malloc(50 * sizeof(char));
            int filesize;
            printf("Enter file name: ");
            scanf("%s", filename);
            printf("Enter file size (in bytes): ");
            scanf("%d", &filesize);

            int start = allocateFile(freelst, freeCount, blocksize, filesize, filename);
            if (start == -1)
            {
                free(filename);
            }
            break;
        }
        case 2:
        {
            char filename[50];
            printf("Enter file name to deallocate: ");
            scanf("%s", filename);

            deallocateFile(freelst, filename, blocksize);
            break;
        }
        case 3:
            showFreeBlocks(freelst);
            break;
        case 4:
            showDirectories(blocksize);
            break;
        case 5:
            printf("Exiting program...\n");
            break;
        default:
            printf("Invalid choice! Please try again.\n");
        }
    } while (choice != 5);

    // Free allocated memory
    for (int i = 0; i <= dirc; i++)
    {
        free(dirs[i].dirname);
    }

    return 0;
}
