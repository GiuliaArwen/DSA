#include <stdio.h>
#include <stdlib.h>
#define FILENAME "val3.txt"

int *readFile(FILE *fp, int *N);
void solve(int *val, int N);
int solveR(int *val, int *best, int N, int k);
int max(int a, int b);
void solveDP(int *val, int N);
void displaySol(int *val, int *best, int N);

int main(void){
    int *val, N;
    FILE *fp = fopen(FILENAME, "r");
    if(fp == NULL) return -1;

    val = readFile(fp, &N);
    printf("N = %d\n", N);
    for(int i=1; i<=N; i++) printf("%d ", val[i]);
    printf("\n");

    solve(val, N);
    solveDP(val, N);

    fclose(fp);
    return 0;
}

int *readFile(FILE *fp, int *N){
    int *val;
    fscanf(fp, "%d\n", N);
    val = calloc((*N+1), sizeof(int));
    val[0] = 0;
    for(int i=1; i<=*N; i++) fscanf(fp, "%d ", &val[i]);
    return val;
}

void solve(int *val, int N){
    int *best;
    best = calloc((N+1), sizeof(int));
    printf("Recursive solution >\tMaximum population covered: %d\n", solveR(val, best, N, N));
}

int solveR(int *val, int *best, int N, int k){
    if(k==0) return 0;
    if(k==1) return val[1];
    return max(solveR(val, best, N, k-1), solveR(val, best, N, k-2) + val[k]);
}

int max(int a, int b){
    if(a>=b) return a;
    else return b;
}

void solveDP(int *val, int N){
    int *best = calloc((N+1), sizeof(int));

    best[1] = val[1];
    for(int i=2; i<=N; i++){
        if(best[i-1] > best[i-2] + val[i]) best[i] = best[i-1];
        else best[i] = best[i-2] + val[i];
    }

    printf("Dynamic programming >\tMaximum population covered: %d\n", best[N]);
    displaySol(val, best, N);
}

void displaySol(int *val, int *best, int N){
    int *sol = calloc((N+1), sizeof(int));
    int i = N;
    sol[1] = 1;
    while(i>=2){
        if(best[i] == best[i-1]){
            sol[i] = 0;
            i--;
        }
        else if(best[i] == best[i-2] + val[i]){
            sol[i] = 1;
            sol[i-1] = 0;
            i -= 2;
        }
    }
    for(i=1; i<=N; i++)
        if(sol[i]) printf("%d ", val[i]);
    printf("\n");
}