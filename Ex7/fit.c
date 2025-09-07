#include <stdio.h>
#include <stdlib.h>

void fitparts(int* part, int* proc, int npart, int nproc){
    int i,j;
    int filled[npart];
    int int_frag=0, ext_frag=0;
    for(i=0;i<npart;i++)
    filled[i] = -1;
    for(i=0;i<nproc;i++){
        for(j=0;j<npart;j++){
            if(filled[j]!=-1) continue;
            int f=0;
            if(proc[i] <= part[j]){
                f=1;
                filled[j] = i;
                printf("Process %d fills partition %d with wastage of %d kb\n",i,j,part[j]-proc[i]);
                int_frag+= part[j] - proc[i];
                break;
            }
            else continue;
            if(f==0)
            printf("Process %d couldn't be fit!\n",i);
        }
    }
    for(i=0;i<npart;i++){
        if(filled[i]==-1){
            ext_frag+= part[i];
        }
    }
    printf("Internal frag: %d kb\nExternal frag: %d kb\n", int_frag, ext_frag);
}

int main(){
    int i,j,np,npr;
    printf("Enter the no. of partitions: ");
    scanf("%d", &np);
    printf("Enter the no. of proc: ");
    scanf("%d", &npr);
    int part[np], proc[npr];
    for(i=0;i<np;i++){
        printf("Partition %d capacity: ", i);
        scanf("%d", &part[i]);
    }
    for(i=0;i<npr;i++){
        printf("Process %d size: ", i);
        scanf("%d", &proc[i]);
    }
    while(1){
    printf("1) First fit 2) Best fit 3) Worst fit 4) Exit\n");
    int ch;
    scanf("%d", &ch);
    switch(ch){
        case 1:{
            fitparts(part, proc, np, npr);
            break;
        }
        case 2:{
            for(i=0;i<np;i++){
                for(j=0;j<np-i-1;j++){
                    if(part[j]>part[j+1]){
                        int temp = part[j];
                        part[j] = part[j+1];
                        part[j+1] = temp;
                    }
                }
            }
            fitparts(part, proc, np, npr);
            break;
        }
        case 3:{
            for(i=0;i<np;i++){
                for(j=0;j<np-i-1;j++){
                    if(part[j]<part[j+1]){
                        int temp = part[j];
                        part[j] = part[j+1];
                        part[j+1] = temp;
                    }
                }
            }
            fitparts(part, proc, np, npr);
            break;
        }
        case 4: {
            printf("Exiting\n");
            exit(0);
        }
        default:{
            printf("Invalid choice!\n");
            
        }
    }
    }
}