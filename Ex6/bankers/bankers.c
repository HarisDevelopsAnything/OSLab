#include <stdio.h>
#include <stdlib.h>

int main(){
    printf("Enter the no. of proc: ");
    int n,i, r,j;
    scanf("%d", &n);
    printf("Enter the no. of resources: ");
    scanf("%d", &r);
    int alloc[n][r], max[n][r], need[n][r];
    for(i=0;i<n;i++){
        printf("P%d:\n",i);
        printf("Enter the following: \n");
        printf("Allocations: ");
        for(j=0;j<r;j++){
            printf("Res. %c: ", j+'A');
            scanf("%d", &alloc[i][j]);
        }
        printf("Max: ");
        for(j=0;j<r;j++){
            printf("Res. %c: ", j+'A');
            scanf("%d", &max[i][j]);
            need[i][j] = max[i][j]-alloc[i][j];
        }
    }
    while(1){
    printf("1) Make safe sequence 2) Check request 3) Exit");
    int ch;
    scanf("%d", &ch);
    switch(ch){
        case 1:{
            int avail[r];
            int fin[n];
            printf("Enter available:\n");
            for(i=0;i<r;i++){
                printf("Res. %c: ", i+'A');
                scanf("%d", &avail[i]);
            }
            int a=0, f=0, ind=0;
            while(a<n*2){
                //check
                int v=0;
                if(fin[ind]==1) continue;
                for(i=0;i<r;i++){
                    if(need[ind][i] > avail[i]) break;
                    v++;
                }
                if(v==r){
                    printf("P%d ", ind);
                    //allocate
                    for(i=0;i<r;i++){
                        avail[i]+=alloc[ind][i];
                    }
                    f++;
                    fin[ind] = 1;
                }
                ind = (ind+1)%n;
                a++;
            }
            if(f==n){
                printf("All processes executed.\n");
            }
            else printf("Execution failed!");
            break;
        }
        case 2:{
            int avail[r];
            printf("Enter available:\n");
            for(i=0;i<r;i++){
                printf("Res. %c: ", i+'A');
                scanf("%d", &avail[i]);
            }
            printf("Enter the process no.: ");
            int no;
            scanf("%d", &no);
            if(no > n || no <0 ){
                printf("Invalid proc. no\n");
                return 0;
            }
            printf("Enter the request amount:\n");
            int req[r];
            for(i=0;i<r;i++){
                printf("Res. %c: ", i+'A');
                scanf("%d", &req[i]);
                if(req[i] > need[no][i] || req[i] > avail[i]){
                    printf("Invalid request for resource!\n");
                    return 0;
                }
                alloc[no][i] += req[i];
                need[no][i] -= req[i];
                avail[i] -= req[i];
            }
            int a=0, f=0, ind=0;
            while(a<n){
                //check
                int v=0;
                for(i=0;i<r;i++){
                    if(need[ind][i] > avail[i]) break;
                    v++;
                }
                if(v==r){
                    printf("P%d ", ind);
                    //allocate
                    for(i=0;i<r;i++){
                        avail[i]+=alloc[ind][i];
                    }
                    f++;
                }
                ind = (ind+1)%n;
                a++;
            }
            if(f==n){
                printf("All processes executed.\n");
            }
            else printf("Execution failed!");
            break;
        }
        default:{
        printf("Bye!\n");
        return 0;
        }
    }
    }
}