#include <stdio.h>
#include <stdlib.h>
#define FILENAME "att2.txt"

typedef struct{
    int start, end;
} activity_t;

activity_t *readFile(FILE *fp, int *N);
void mergeSort(activity_t *act, int N);
void mergeSortR(activity_t *act, activity_t *act1, int l, int r);
void merge(activity_t *act, activity_t *act1, int l, int m, int r);
void powerset(activity_t *act, int N);
void powersetR(int pos, int N, activity_t *act, int *sol, int *best, int currLen, int *bestLen);
int overlap(int N, activity_t *act, int *sol, int pos);
void divideEtImpera(activity_t *act, int N);
int divideEtImperaR(activity_t *act, int pos, int *q);
int incompatible(activity_t *act, int N, int i, int j);
void dynamicProgramming(activity_t *act, int N);
void displaySol(activity_t *act, int pos, int *best, int *q);
int min(int a, int b);
int max(int a, int b);

int main(void){
    int N;
    activity_t *act;
    FILE *fp = fopen(FILENAME, "r");
    if(fp == NULL) return -1;

    act = readFile(fp, &N);
    printf("Original activities:\n");
    for(int i=1; i<=N; i++) printf("(%d,%d) ", act[i].start, act[i].end);
    printf("\n\n");

    mergeSort(act, N);
    printf("Activities sorted by ascending ending:\n");
    for(int i=1; i<=N; i++) printf("(%d,%d) ", act[i].start, act[i].end);
    printf("\n");

    printf("\nRecursive powerset solution:\n");
    powerset(act, N);

    printf("\nRecursive divide et impera solution:\n");
    divideEtImpera(act, N);

    printf("\nDynamic programming solution:\n");
    dynamicProgramming(act, N);

    fclose(fp);
    return 0;
}

activity_t *readFile(FILE *fp, int *N){
    activity_t *act;
    fscanf(fp, "%d\n", N);
    act = calloc((*N+1), sizeof(activity_t));
    for(int i=1; i<=*N; i++) fscanf(fp, "%d %d\n", &act[i].start, &act[i].end);                 // first index is used as dummy
    return act;
}

void mergeSort(activity_t *act, int N){
    int l=0, r=N-1;
    activity_t *act1 = (activity_t *) malloc((N+1)*sizeof(activity_t));                         // auxiliary array (sorting not in place)
    mergeSortR(act, act1, l, r);
}

void mergeSortR(activity_t *act, activity_t *act1, int l, int r){
    if(r <= l) return;
    int m = (l+r)/2;
    mergeSortR(act, act1, l, m);
    mergeSortR(act, act1, m+1, r);
    merge(act, act1, l, m, r);
}

void merge(activity_t *act, activity_t *act1, int l, int m, int r){
    int i = l, j = m+1, k;
    for(k=l; k <= r; k++){
        if(i > m) act1[k] = act[j++];                                                           // stores in right half array and then increments
        else if(j > r) act1[k] = act[i++];                                                      // stores in left half array and then increments
        else if(act[i].end <= act1[j].end) act1[k] = act[i++];                                  // compares the two halves
        else act1[k] = act[j++];
    }
    for(k=l; k<=r; k++) act[k] = act1[k];                                                       // overwrites the original array
    return;
}

void powerset(activity_t *act, int N){                                                          // powerset recursive approach
    int *sol = calloc((N+1), sizeof(int));
    int *best = calloc((N+1), sizeof(int));
    int len = 0;

    powersetR(1, N, act, sol, best, 0, &len);                                                   // pos=1 as it skips the dummy
    for(int i=1; i<=N; i++)
        if(best[i]) printf("(%d,%d) ", act[i].start, act[i].end);
    printf("\nBest length: %d\n", len);

    free(sol);
    free(best);
}

void powersetR(int pos, int N, activity_t *act, int *sol, int *best, int currLen, int *bestLen){
    if(pos > N) {
        if(*bestLen < currLen){
            *bestLen = currLen;
            for(int i=1; i<=N; i++) best[i] = sol[i];
        }
        return;
    }
    sol[pos] = 0;                                                                               // tries not including the current element
    powersetR(pos+1, N, act, sol, best, currLen, bestLen);
    if(!overlap(N, act, sol, pos)){                                                             // first checks validity
        sol[pos] = 1;                                                                           // then tries including the element
        powersetR(pos+1, N, act, sol, best, currLen + (act[pos].end-act[pos].start), bestLen);  // adds current duration to length
    }
}

int overlap(int N, activity_t *act, int *sol, int pos){
    for(int i=0; i<pos; i++){
        if(sol[i] != 0){
            if(act[i].start < act[pos].end && act[pos].start < act[i].end)                      // compares current element with previous ones included
                return 1;
        }
    }
    return 0;
}

void divideEtImpera(activity_t *act, int N){                                                    // divide et impera recursive approach
    int res=0;
    int *sol = calloc((N+1), sizeof(int));
    for(int i=2; i<=N; i++)                                                                     // outer loop iterates over the original array
        for(int j=1; j<i; j++)                                                                  // inner loop compares with previous elements of the same array
            if(incompatible(act, N, i, j)==0) sol[i] = j;                                       // only stores index if valid
    res = divideEtImperaR(act, N, sol);
    printf("Best length: %d\n", res);
}

int divideEtImperaR(activity_t *act, int pos, int *sol){
    if(pos==0) return 0;
    return max(divideEtImperaR(act, sol[pos], sol)+(act[pos].end-act[pos].start), divideEtImperaR(act, pos-1, sol));
}

int incompatible(activity_t *act, int N, int i, int j){                                         // checks compatibility with previously included elements
    if(act[i].start < act[j].end && act[j].start < act[i].end) return 1;
    return 0;
}

void dynamicProgramming(activity_t *act, int N){                                                // dynamic proramming approach
    int *sol = calloc((N+1), sizeof(int));
    int *best = calloc((N+1), sizeof(int));
    for(int i=2; i<=N; i++)
        for(int j=1; j<i; j++)
            if(incompatible(act, N, i, j)==0) sol[i]=j;
    best[0] = 0;
    for(int i=1; i<=N; i++)
        best[i] = max((act[i].end-act[i].start)+best[sol[i]], best[i-1]);                       // gets the maximum out of inclusion and exclusion
    displaySol(act, N, best, sol);
    printf("\nBest length: %d\n", best[N]);
    return;
}

void displaySol(activity_t *act, int pos, int *best, int *sol){
    if(pos==0) return;
    if((act[pos].end-act[pos].start)+best[sol[pos]] >= best[pos-1]){                            // includes
        displaySol(act, sol[pos], best, sol);
        printf("(%d,%d) ", act[pos].start, act[pos].end);
    }
    else displaySol(act, pos-1, best, sol);                                                     // excludes
}

int min(int a, int b){
    if(a < b) return a;
    return b;
}

int max(int a, int b){
    if(a > b) return a;
    return b;
}