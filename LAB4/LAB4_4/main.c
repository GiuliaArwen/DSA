#include <stdio.h>
#include <stdlib.h>
#define MAX 4
#define FILENAME "test_set.txt"

void solve(char *gems, int *gemsQTY, int *gemsVAL, int max, int maxrep);
int rep_disp(int pos, int val, int *sol, int *gemsQTY, int *gemsVAL, int *gemsUSED, int *gemsREP, int max, int prev, int *bestSol, int *bestVal, int *bestL, int maxrep);

int main(void){
    int N, gemsQTY[MAX], gemsVAL[MAX], max, maxrep;
    char gems[MAX] = {'S', 'R', 'T', 'E'}, *gemsNAMES[MAX] = {"Sapphires", "Rubies", "Topazes", "Emeralds"};

    FILE *fp = fopen(FILENAME, "r");
    if (fp == NULL) return -1;

    fscanf(fp, "%d", &N);
    for(int i=0; i<N; i++){
        printf("\nTEST #%d\n", i+1);
        max=0;
        for(int j=0; j<MAX; j++){
            fscanf(fp, "%d", &gemsQTY[j]);
            max += gemsQTY[j];
        }
        for(int j=0; j<MAX; j++){
            fscanf(fp, "%d", &gemsVAL[j]);
            printf("%s = %d [value: %d] ", gemsNAMES[j], gemsQTY[j], gemsVAL[j]);
        }
        fscanf(fp, "%d", &maxrep);
        printf("\nTOT = %d {maxrep = %d}\n", max, maxrep);
        solve(gems, gemsQTY, gemsVAL, max, maxrep);
    }
    fclose(fp);
    return 0;
}

void solve(char *gems, int *gemsQTY, int *gemsVAL, int max, int maxrep){
    int *sol = calloc(max, sizeof(int)), *bestSol = calloc(max, sizeof(int));
    int *gemsUSED = calloc(MAX, sizeof(int)), *gemsREP = calloc(MAX, sizeof(int));
    int bestL = -1, bestVal = -1;

    rep_disp(0, 0, sol, gemsQTY, gemsVAL, gemsUSED, gemsREP, max, -1, bestSol, &bestVal, &bestL, maxrep);

    printf("Best solution found having %d value and %d gems!\n", bestVal, bestL);
    for(int i=0; i<bestL; i++) printf("%c", gems[bestSol[i]]);
    printf("\n");
    free(gemsUSED);
    free(gemsREP);
    free(bestSol);
    free(sol);
}

int rep_disp(int pos, int val, int *sol, int *gemsQTY, int *gemsVAL, int *gemsUSED, int *gemsREP, int max, int prev, int *bestSol, int *bestVal, int *bestL, int maxrep){
    int prevRep=0;
    if(val > *bestVal){
        if(gemsUSED[0] <= gemsUSED[3]){
            *bestVal = val;
            *bestL = pos;
            for(int i=0; i<pos; i++) bestSol[i] = sol[i];
        }
    }
    if(pos>=max) return 1;
    for(int i=0; i<MAX; i++){
        if(gemsQTY[i]-gemsUSED[i] > 0){
            if(prev!=-1){
                if((prev == 0 || prev == 2) && (i != 0 && i != 1)) continue;
                if((prev == 1 || prev == 3) && (i != 3 && i != 2)) continue;
            }
            if(prev == i)
                if(gemsREP[i]+1 > maxrep) continue;
            if(prev == -1) gemsREP[i] = 1;
            else{
                if(prev == i) gemsREP[i]++;
                else{
                    gemsREP[i] = 1;
                    prevRep = gemsREP[prev];
                    gemsREP[prev] = 0;
                }
            }
            gemsUSED[i]++;
            sol[pos] = i;
            if(rep_disp(pos+1, val + gemsVAL[i], sol, gemsQTY, gemsVAL, gemsUSED, gemsREP, max, i, bestSol, bestVal, bestL, maxrep)) return 1;
            
            gemsUSED[i]--;
            if(prev == -1) gemsREP[i] = 0;
            else{
                if(prev == i) gemsREP[i]--;
                else{
                    gemsREP[i] = 0;
                    gemsREP[prev] = prevRep;
                }
            }
        }
    }
    return 0;
}