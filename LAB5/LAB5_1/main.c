#include <stdio.h>
#include <stdlib.h>
#define FILENAME "act1.txt"

typedef struct{
    int start;
    int end;
} activity_t;

activity_t *readFile(FILE *fp, int *N);
void actSel(int N, activity_t *act);
void actSelR(int pos, int N, activity_t *act, int *sol, int *bestSol, int time, int *bestTime);
int overlap(activity_t *act, int *sol, int pos);

int main(void){
    int N;
    activity_t *act;

    FILE *fp = fopen(FILENAME, "r");
    if(fp == NULL) return -1;

    act = readFile(fp, &N);
    actSel(N, act);

    fclose(fp);
    return 0;
}

activity_t *readFile(FILE *fp, int *N){
    activity_t *act;
    fscanf(fp, "%d", N);
    act = calloc(*N, sizeof(activity_t));
    for(int i=0; i<*N; i++) fscanf(fp, "%d %d", &act[i].start, &act[i].end);
    return act;
}

void actSel(int N, activity_t *act){                                    // wrapper for recursive function
    int *sol = calloc(N, sizeof(int));
    int *bestSol = calloc(N, sizeof(int));
    int duration = 0;

    actSelR(0, N, act, sol, bestSol, 0, &duration);
    printf("Solution with overall longest duration: %d\n", duration);
    for(int i=0; i<N; i++)                                              
        if(bestSol[i]) printf(" (%d,%d) ", act[i].start, act[i].end);   // bestSol is set to 1 only if the i-th activity is included
    printf("\n");
    
    free(sol);
    free(bestSol);
    return;
}

void actSelR(int pos, int N, activity_t *act, int *sol, int *bestSol, int time, int *bestTime){
    if(pos >= N){                                                       // exit condition: end of array reached
        if(time > *bestTime){                                           // updates best solution if a better match is found
            *bestTime = time;
            for(int i=0; i<N; i++) bestSol[i] = sol[i];
        }
        return;
    }

    sol[pos] = 0;                                                       // tries excluding the current solution
    actSelR(pos+1, N, act, sol, bestSol, time, bestTime);

    if(!overlap(act, sol, pos)){                                        // tries including the current solution as long at it doesn't overlap
        sol[pos] = 1;
        actSelR(pos+1, N, act, sol, bestSol, time + (act[pos].end - act[pos].start), bestTime);
    }
    return;
}

int overlap(activity_t *act, int *sol, int pos){
    for(int i=0; i<pos; i++){
        if(sol[i] != 0)                                                 // only considers activities included in the solution
            if(act[i].start < act[pos].end && act[pos].start < act[i].end) return 1;
    }
    return 0;
}