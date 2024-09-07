#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILENAME "elements.txt"
#define DIAG 3
#define MAX 5
#define LEN 100
#define DD 11
#define DP 33

typedef struct{
    char *name;
    int type;
    int in, out;
    int prev;
    int final;
    float value;
    int difficulty;
} element_t;

typedef struct{
    element_t *elements;
    int nEl;
} elements_t;

typedef struct{
    int *el;
    int nEl;
    float value;
    int difficulty;
} diagonal_t;

typedef struct{
    diagonal_t *diagonals;
    int nDiag, maxDiag;
} diagonals_t;

diagonals_t *initDiagonals();
elements_t *readElements();
void generateDiagonals(elements_t *E, diagonals_t *D, int dd);
void generatePrograms(elements_t *E, diagonals_t *D, int dp);
void rep_disp(elements_t *E, diagonals_t *D, int dd, int pos, int diff, int acro, int dir, int fin, int *diag);
void rep_comb(elements_t *E, diagonals_t *D, int dp, int pos, int diff, int *sol, int *bestSol, float *bestVal, int *bestBonus, int *num, int start);
int checkValidity(elements_t *E, diagonals_t *D, int *sol, int n, float *val, int *bon);

int main(void){
    int dd = DD, dp = DP;
    diagonals_t *D = initDiagonals();
    elements_t *E = readElements();
    printf("Generating diagonals...\n");
    generateDiagonals(E,D,dd);
    printf("%d diagonals available.\n", D->nDiag);
    printf("Generating programs...\n");
    generatePrograms(E,D,dp);
    return 0;
}

diagonals_t *initDiagonals(){
    diagonals_t *D = malloc(1*sizeof(*D));
    D->diagonals = calloc(1, sizeof(diagonal_t));
    D->nDiag = 0;
    D->maxDiag = 1;
    return D;
}

elements_t *readElements(){
    char tmpName[LEN];
    FILE *fp = fopen(FILENAME, "r");
    if(fp == NULL) return NULL;
    elements_t *E = malloc(1*sizeof(*E));
    if(E == NULL) return NULL;
    fscanf(fp, "%d", &E->nEl);
    E->elements = calloc(E->nEl, sizeof(element_t));
    if(E->elements == NULL) return NULL;

    for(int i=0; i<E->nEl; i++){
        fscanf(fp, "%s %d %d %d %d %d %f %d", tmpName, &E->elements[i].type, &E->elements[i].in, &E->elements[i].out, &E->elements[i].prev, &E->elements[i].final, &E->elements[i].value, &E->elements[i].difficulty);
        E->elements[i].name = strdup(tmpName);
    }
    return E;
}

void generateDiagonals(elements_t *E, diagonals_t *D, int dd){
    int *diag = calloc(MAX, sizeof(int));
    rep_disp(E, D, dd, 0, 0, 0, 1, 0, diag);
}

void rep_disp(elements_t *E, diagonals_t *D, int dd, int pos, int diff, int acro, int dir, int fin, int *diag){
    if(pos>0){
        if(acro>0){
            if(D->nDiag == D->maxDiag){
                D->maxDiag *= 2;
                D->diagonals = realloc(D->diagonals, D->maxDiag*sizeof(diagonal_t));
                if(D->diagonals == NULL) exit(-1);
            }
            D->diagonals[D->nDiag].difficulty = diff;
            D->diagonals[D->nDiag].nEl = pos;
            D->diagonals[D->nDiag].el = malloc(pos * sizeof(int));
            D->diagonals[D->nDiag].value = 0.0;
            for(int i=0; i<pos; i++){
                D->diagonals[D->nDiag].value += E->elements[diag[i]].value;
                D->diagonals[D->nDiag].el[i] = diag[i];
            }
            D->nDiag++;
        }
    }
    if(pos >= MAX || fin) return;

    for(int i=0; i<E->nEl; i++){
        if(diff + E->elements[i].difficulty > DD) continue;
        if(dir != E->elements[i].in) continue;
        if(pos == 0 && E->elements[i].prev) continue;
        diag[pos] = i;
        rep_disp(E, D, dd, pos+1, diff + E->elements[i].difficulty, acro + E->elements[i].type, E->elements[i].out, E->elements[i].final, diag);
    }
}

void generatePrograms(elements_t *E, diagonals_t *D, int dp){
    int d, bonus, num = 0;
    float bestValue = -1.0;
    int *sol = malloc(DIAG * sizeof(int));
    int *bestSol = malloc(DIAG * sizeof(int));
    rep_comb(E, D, dp, 0, 0, sol, bestSol, &bestValue, &bonus, &num, 0);
    if(num > 0){
        printf("TOT = %f\n", bestValue);
        for(int i=0; i<num; i++){
            d = bestSol[i];
            printf("DIAG %d > %.3f %s\n", d, D->diagonals[d].value, ((bonus == i) ? "* 1.5 (BONUS)" : ""));
            for(int j=0; j < D->diagonals[d].nEl; j++)
                printf("%s ", E->elements[D->diagonals[d].el[j]].name);
            printf("\n");
        }
    }
}

void rep_comb(elements_t *E, diagonals_t *D, int dp, int pos, int diff, int *sol, int *bestSol, float *bestVal, int *bestBonus, int *num, int start){
    int bonus;
    float val = 0.0;
    if(pos >= DIAG){
        if(checkValidity(E, D, sol, pos, &val, &bonus))
            if(val > *bestVal){
                *num = pos;
                *bestVal = val;
                *bestBonus = bonus;
                memcpy(bestSol, sol, pos*sizeof(int));
            }
        return;
    }
    for(int i=start; i < D->nDiag; i++){
        if(diff + D->diagonals[i].difficulty > dp) continue;
        sol[pos] = i;
        rep_comb(E, D, dp, pos+1, diff + D->diagonals[i].difficulty, sol, bestSol, bestVal, bestBonus, num, i);
    }
}

int checkValidity(elements_t *E, diagonals_t *D, int *sol, int n, float *val, int *bon){
    int bonus = -1;
    int fwd=0, bwd=0, seq=0;
    for(int i=0; i<n; i++){
        int dseq = 0;
        for(int j=0; j<D->diagonals[sol[i]].nEl; j++){
            if(E->elements[D->diagonals[sol[i]].el[j]].type == 2){
                fwd = 1;
                dseq++;
            }
            else if(E->elements[D->diagonals[sol[i]].el[j]].type == 1){
                bwd = 1;
                dseq++;
            }
            else
                dseq = 0;
            if(j == (D->diagonals[sol[i]].nEl-1) && E->elements[D->diagonals[sol[i]].el[j]].type > 0 && E->elements[D->diagonals[sol[i]].el[j]].difficulty > 7)
                if(bonus == -1 || (D->diagonals[sol[bonus]].value < D->diagonals[sol[i]].value))
                    bonus = i;
            if(dseq >= 2) seq = 1;
        }
    }
    if(fwd && bwd && seq){
        for(int i=0; i<n; i++)
            (*val) += D->diagonals[sol[i]].value * ((i == bonus) ? 1.50 : 1.00);
        *bon = bonus;
        return 1;
    }
    return 0;
}