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
    printf("\n1) Make safe sequence 2) Check request 3) Exit\nChoice: ");
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
            // Initialize fin array
            for(i=0;i<n;i++){
                fin[i] = 0;
            }
            int a=0, f=0, ind=0;
            while(a<n*n){  // Changed from n*2 to n*n to ensure enough iterations
                //check
                int v=0;
                if(fin[ind]==1) {
                    ind = (ind+1)%n;
                    a++;
                    continue;
                }
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
                printf("\nAll processes executed.\n");
            }
            else printf("\nExecution failed!");
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
            if(no >= n || no < 0){  // Changed > to >= since array is 0-indexed
                printf("Invalid proc. no\n");
                break;  // Changed from return 0 to break
            }
            printf("Enter the request amount:\n");
            int req[r];
            int valid = 1;
            for(i=0;i<r;i++){
                printf("Res. %c: ", i+'A');
                scanf("%d", &req[i]);
                if(req[i] > need[no][i] || req[i] > avail[i]){
                    printf("Invalid request for resource!\n");
                    valid = 0;
                    break;
                }
            }
            if(!valid) break;  // Skip processing if invalid request
            
            // Temporarily grant the request
            for(i=0;i<r;i++){
                alloc[no][i] += req[i];
                need[no][i] -= req[i];
                avail[i] -= req[i];
            }
            
            // Check safety
            int fin[n];
            for(i=0;i<n;i++){
                fin[i] = 0;
            }
            int a=0, f=0, ind=0;
            while(a<n*n){  // Changed from just n to n*n
                //check
                int v=0;
                if(fin[ind]==1) {
                    ind = (ind+1)%n;
                    a++;
                    continue;
                }
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
                printf("\nRequest granted! All processes executed.\n");
            }
            else {
                printf("\nRequest denied! Execution failed!");
                // Rollback the request
                for(i=0;i<r;i++){
                    alloc[no][i] -= req[i];
                    need[no][i] += req[i];
                    avail[i] += req[i];
                }
            }
            break;
        }
        default:{
        printf("Bye!\n");
        return 0;
        }
    }
    }
}