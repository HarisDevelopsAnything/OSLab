#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void sort(int* arr, int n){
        for(int i=0;i<n;i++){
                for(int j=0;j<n-i-1;j++){
                        if(arr[j] > arr[j+1]){
                                int temp = arr[j];
                                arr[j] = arr[j+1];
                                arr[j+1] = temp;
                        }
                }
        }
}

void FCFS(int* cylst, int reqcnt, int totcy, int inithp){
        int total = 0;
        int pre = inithp;
        for(int i=0;i<reqcnt;i++){
              total+= abs(cylst[i]-pre);
              printf("%d to %d: %d\n", pre, cylst[i], abs(cylst[i]-pre));
              pre = cylst[i];
        }
        printf("Total distance: %d\n", total);
}

void SSTF(int* cylst, int reqcnt, int totcy, int inithp){
        int total=0;
        int min = 999;
        int pre = inithp;
        sort(cylst, reqcnt);
        //finding first
        int curr = 0;
        for(int i=0;i<reqcnt;i++){
                if(abs(cylst[i]-inithp) > min){
                        curr = i;
                        min = abs(cylst[i]-inithp);
                }
        }
        printf("%d to %d: %d\n", pre, cylst[curr], abs(cylst[i]-inithp));
        int* finished= (int*)calloc(sizeof(int),reqcnt);
        finished[curr] = 1;
        int finc = 1;
        while(finc < reqcnt){
                if(curr-1 < 0 || (cylst[curr] - cylst[curr-1] > cylst[curr+1] - cylst[curr]) && finished[curr+1]==0){
                        curr = curr+1;
                        printf("%d to %d: %d\n", curr-1, curr, cylst[curr] - cylst[curr-1]);
                        finished[curr] = 1;
                        finc++;
                        total += cylst[curr] - cylst[curr-1];
                }
                else{
                        curr = curr-1;
                        printf("%d to %d: %d\n", curr+1, curr, cylst[curr+1] - cylst[curr]);
                        finished[curr] = 1;
                        finc++;
                        total += cylst[curr+1] - cylst[curr];
                }
        }
        printf("Total distance: %d\n", total);
}
void SCAN(int* cylst, int reqcnt, int totcy, int inithp){
        int total=0;
        int min = 999;
        int pre = inithp;
        sort(cylst, reqcnt);
        //finding first
        int curr = 0;
        for(int i=0;i<reqcnt;i++){
                if(cylst[i] > inithp){
                        curr = i;
                        min = abs(cylst[i]-inithp);
                }
        }
        printf("%d to %d: %d\n", pre, cylst[curr], abs(cylst[i]-inithp));
        int* finished= (int*)calloc(sizeof(int),reqcnt);
        finished[curr] = 1;
        int finc = 1;
        while(finc < reqcnt){

        }
        printf("Total distance: %d\n", total);
}
int main(){
        int na, nc;
        printf("Enter the number of accesses: ");
        scanf("%d", &na);
        printf("Enter the number of cylinders: ");
        scanf("%d", &nc);
        int acc[na];
        printf("Enter the accesses: ");
        for(int i=0;i<na;i++){
                scanf("%d", &acc[i]);
        }
        int ch;
        int hp;
        while(1){
                printf("1) FCFS 2) SSTF 3) SCAN 4) C-SCAN 5) LOOK 6) C-LOOK\n");
                scanf("%d", &ch);
                printf("Enter the head position: ");
                scanf("%d", &hp);
                switch(ch){
                        case 1: {
                                int total = 0;
                                int pre = hp;
                                for(int i=0;i<na;i++){
                                        total+= abs(acc[i]-pre);
                                        printf("%d to %d: %d\n", pre, acc[i], abs(acc[i]-pre));
                                        pre = acc[i];
                                }
                                printf("Total distance: %d\n", total);
                                break;
                                }
                        case 2: {
                                        SSTF(acc, na, nc, hp);
                                        break;
                                }
                }
        }
}

