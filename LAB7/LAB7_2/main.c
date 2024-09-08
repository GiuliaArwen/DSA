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
    int prev, final;
    float value;
    int difficulty;
} element_t;

typedef struct{
    element_t *elements;
    int nE;
} elements_t;

typedef struct{
    int *indexE;
    int nE;
    float value;
    int difficulty;
} diagonal_t;

typedef struct{
    diagonal_t *diagonals;
    int nD, maxD;
} diagonals_t;

diagonals_t *initDiagonals();
elements_t *readElements();
void generateDiagonals(elements_t *E, diagonals_t *D, int dd);
void generatePrograms(elements_t *E, diagonals_t *D, int dp);
void rep_disp(elements_t *E, diagonals_t *D, int dd, int pos, int diff, int type, int dir, int fin, int *sol);
void rep_comb(elements_t *E, diagonals_t *D, int dp, int pos, int diff, int *sol, int *bestSol, float *bestVal, int *bestBonus, int *num, int start);
int checkValidity(elements_t *E, diagonals_t *D, int *sol, int n, float *val, int *B);

int main(void){
    diagonals_t *D = initDiagonals();
    elements_t *E = readElements();

    printf("Generating diagonals...\n");
    generateDiagonals(E,D,DD);
    printf("%d diagonals available.\n", D->nD);

    printf("Generating programs...\n");
    generatePrograms(E,D,DP);

    return 0;
}

diagonals_t *initDiagonals(){
    diagonals_t *D = malloc(1*sizeof(*D));
    D->diagonals = calloc(1, sizeof(diagonal_t));
    D->nD = 0;
    D->maxD = 1;
    return D;
}

elements_t *readElements(){
    char tmpName[LEN];

    elements_t *E = malloc(1*sizeof(*E));
    if(E == NULL) return NULL;

    FILE *fp = fopen(FILENAME, "r");
    if(fp == NULL) return NULL;

    fscanf(fp, "%d", &E->nE);
    E->elements = calloc(E->nE, sizeof(element_t));
    if(E->elements == NULL) return NULL;

    for(int i=0; i < E->nE; i++){
        fscanf(fp, "%s %d %d %d %d %d %f %d", tmpName, &E->elements[i].type, &E->elements[i].in, &E->elements[i].out, &E->elements[i].prev, &E->elements[i].final, &E->elements[i].value, &E->elements[i].difficulty);
        E->elements[i].name = strdup(tmpName);
    }
    return E;
}

void generateDiagonals(elements_t *E, diagonals_t *D, int dd){
    int *sol = calloc(MAX, sizeof(int));
    rep_disp(E, D, dd, 0, 0, 0, 1, 0, sol);
}

void rep_disp(elements_t *E, diagonals_t *D, int dd, int pos, int diff, int type, int dir, int fin, int *sol){
    if(pos>0){                                                          // if an element is added
        if(type>0){                                                     // checks for acrobatic type [1], [2]
            if(D->nD == D->maxD){                                       // if reached the maximum (initially set to 1), realloc
                D->maxD *= 2;
                D->diagonals = realloc(D->diagonals, D->maxD*sizeof(diagonal_t));
                if(D->diagonals == NULL) exit(-1);
            }
            D->diagonals[D->nD].difficulty = diff;                      // creates a new diagonal with stored values
            D->diagonals[D->nD].nE = pos;
            D->diagonals[D->nD].indexE = malloc(pos * sizeof(int));
            D->diagonals[D->nD].value = 0.0;
            for(int i=0; i<pos; i++){                                   // for each diagonal, updates value
                D->diagonals[D->nD].value += E->elements[sol[i]].value;
                D->diagonals[D->nD].indexE[i] = sol[i];                 // stores the index for the element
            }
            D->nD++;                                                    // nD used as iteration index for chosen diagonals
        }
    }
    if(pos >= MAX || fin) return;                                       // returns if reached 3 diagonals or final element

    for(int i=0; i<E->nE; i++){
        if(diff + E->elements[i].difficulty > DD) continue;             // skips if exceeded max diagonal difficulty
        if(dir != E->elements[i].in) continue;                          // skips if the entry/exit directions don't match
        if(pos == 0 && E->elements[i].prev) continue;                   // skips if the first element has to follow another one
        sol[pos] = i;
        rep_disp(E, D, dd, pos+1, diff + E->elements[i].difficulty, type + E->elements[i].type, E->elements[i].out, E->elements[i].final, sol);
    }
}

void generatePrograms(elements_t *E, diagonals_t *D, int dp){
    int d;                                                              // d stores the index of the diagonal
    int bonus;                                                          // bonus stores the index of the diagonal with bonus
    int N = 0;                                                          // N is the number of diagonals included in the best program
    float bestValue = -1.0;                                          
    int *sol = malloc(DIAG * sizeof(int));
    int *bestSol = malloc(DIAG * sizeof(int));
    rep_comb(E, D, dp, 0, 0, sol, bestSol, &bestValue, &bonus, &N, 0);
    if(N > 0){                                                          // best program is found
        printf("TOT = %f\n", bestValue);
        for(int i=0; i<N; i++){
            d = bestSol[i];
            printf("DIAG %d > %.3f %s\n", d, D->diagonals[d].value, ((bonus == i) ? "* 1.5 (BONUS)" : ""));
            for(int j=0; j < D->diagonals[d].nE; j++)
                printf("%s ", E->elements[D->diagonals[d].indexE[j]].name);
            printf("\n");
        }
    }
}

void rep_comb(elements_t *E, diagonals_t *D, int dp, int pos, int diff, int *sol, int *bestSol, float *bestVal, int *bestBonus, int *num, int start){
    int bonus;
    float val = 0.0;
    if(pos >= DIAG){                                                    // exit condition: reached maximum diag number
        if(checkValidity(E, D, sol, pos, &val, &bonus))                 // checks validity
            if(val > *bestVal){                                         // updates best case
                *num = pos;
                *bestVal = val;
                *bestBonus = bonus;
                memcpy(bestSol, sol, pos*sizeof(int));                  // copies the contents of sol in bestSol
            }
        return;
    }
    for(int i=start; i < D->nD; i++){
        if(diff + D->diagonals[i].difficulty > dp) continue;            // skips if exceeded maximum program difficulty
        sol[pos] = i;                                                   // otherwise stores solution
        rep_comb(E, D, dp, pos+1, diff + D->diagonals[i].difficulty, sol, bestSol, bestVal, bestBonus, num, i);
    }
}

int checkValidity(elements_t *E, diagonals_t *D, int *sol, int n, float *val, int *B){
    int bonus = -1;
    int fwd=0, bwd=0, seq=0;                                            // flag to consider consecutive acros
    for(int i=0; i<n; i++){                                             // iterates over diagonals
        int dseq = 0;                                                   // counter for seq
        for(int j=0; j<D->diagonals[sol[i]].nE; j++){                   // iterates over elements
            if(E->elements[D->diagonals[sol[i]].indexE[j]].type == 2){
                fwd = 1;
                dseq++;
            }
            else if(E->elements[D->diagonals[sol[i]].indexE[j]].type == 1){
                bwd = 1;
                dseq++;
            }
            else
                dseq = 0;
            if(j == (D->diagonals[sol[i]].nE-1) && E->elements[D->diagonals[sol[i]].indexE[j]].type > 0 && E->elements[D->diagonals[sol[i]].indexE[j]].difficulty >= 8)
                if(bonus == -1 || (D->diagonals[sol[bonus]].value < D->diagonals[sol[i]].value))
                    bonus = i;
            if(dseq >= 2) seq = 1;                                      // updates flag
        }
    }
    if(fwd && bwd && seq){
        for(int i=0; i<n; i++)
            (*val) += D->diagonals[sol[i]].value * ((i == bonus) ? 1.50 : 1.00);
        *B = bonus;
        return 1;
    }
    return 0;
}