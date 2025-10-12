#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define blocks 100

struct directory
{
	int startblock;
	char *dirname;
	int size;
} dirs[100];

static int dirc = -1;

int isFree(int *freelst, int freeCount, int block)
{
	for (int i = 0; i < freeCount; i++)
	{
		if (freelst[i] == block)
			return 1;
	}
	return 0;
}

int allocateFile(int *freelst, int freeCount, int blocksize, int filesize, char *name)
{
	int nb = ceil(filesize / blocksize);
	if (nb > freeCount)
	{
		printf("Not enough blocks!\n");
		return -1;
	}
	int cont = 0;
	int start = 0;
	for (int i = 0; i < freeCount; i++)
	{
		if (freelst[i] == 1)
			cont++;
		if (cont == nb)
		{
			start = i - nb + 1;
			int j = i;
			while (j >= start)
			{
				freelst[j] = 0;
			}
			struct directory newdir;
			newdir.startblock = start;
			newdir.dirname = name;
			newdir.size = filesize;
			dirc++;
			dirs[dirc] = newdir;
			return start;
		}
	}
	return -1;
}

int deallocateFile(int *freelst, int start, int blocksize, int filesize)
{
	int nb = ceil(filesize / blocksize);
	for (int i = start; i < start + nb; i++)
	{
		freelst[i] = 1;
	}
	dirc--;
}

void showFreeBlocks(int *freelst)
{
	for (int i = 0; i < blocks; i++)
	{
		if (freelst[i] == 1)
			printf("%d ", i);
	}
	printf("\n");
}

void showDirectories(int blocksize)
{
	if (dirc == -1)
	{
		printf("No directories\n");
		return;
	}
	for (int i = 0; i <= dirc; i++)
	{
		printf("File name: %s", dirs[i].dirname);
		printf("File size: %s", dirs[i].size);
		int nb = ceil(dirs[i].size / blocksize);
		printf("Blocks occupied: ");
		for (int j = dirs[i].startblock; j < dirs[i].startblock + nb; j++)
			printf("%d ", j);
		printf("\n");
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
	}

	printf("Enter block size: ");
	scanf("%d", &blocksize);

	int choice;
	do
	{
		printf("\n--- Menu ---\n");
		printf("1. Allocate file\n");
		printf("2. Deallocate file\n");
		printf("3. Show free blocks\n");
		printf("4. Show directories\n");
		printf("5. Exit\n");
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
			printf("Enter file size: ");
			scanf("%d", &filesize);

			int start = allocateFile(freelst, freeCount, blocksize, filesize, filename);
			if (start != -1)
			{
				printf("File allocated starting at block %d\n", start);
				freeCount -= ceil((double)filesize / blocksize);
			}
			break;
		}
		case 2:
		{
			int start, filesize;
			printf("Enter starting block: ");
			scanf("%d", &start);
			printf("Enter file size: ");
			scanf("%d", &filesize);

			deallocateFile(freelst, start, blocksize, filesize);
			freeCount += ceil((double)filesize / blocksize);
			printf("File deallocated\n");
			break;
		}
		case 3:
			printf("Free blocks: ");
			showFreeBlocks(freelst);
			break;
		case 4:
			showDirectories(blocksize);
			break;
		case 5:
			printf("Exiting...\n");
			break;
		default:
			printf("Invalid choice!\n");
		}
	} while (choice != 5);

	return 0;
}
