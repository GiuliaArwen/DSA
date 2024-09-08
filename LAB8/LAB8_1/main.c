#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILENAME "elements.txt"
#define LEN 100
#define DIAG 3
#define MAX 5
#define DD 11
#define DP 33
#define MEDVAL (((float)DD)/MAX)

typedef struct{
    char name[LEN];
    int type;
    int in, out;
    int prev, final;
    float val;
    float relVal;
    int difficulty;
    int taken;
} element_t;

typedef struct{
    element_t *elements;
    int nE;
} elements_t;

typedef struct{
    int indexE[MAX];
    int nE;
    float val;
    int difficulty;
} diagonal_t;

typedef struct{
    diagonal_t diagonals[DIAG];
    int difficulty;
} diagonals_t;

typedef enum{value_c, relative_value_c, medium_value_c} criteria_e;
int chosenC;

elements_t *readElements();
void greedy(elements_t *E);
int completeDiag(diagonal_t *d, elements_t *E, int diff0, int acro);
int nextBest(elements_t *E, int first, int dir, int diff, int diffTot, int acro, int changeDir);
int bestElement(element_t *e1, element_t *e2);
int verifyAcro(diagonal_t *d, elements_t *E, int i, int acro);
int checkBonus(diagonals_t *program, elements_t *E);

int main(void){
    elements_t *E = readElements();

    printf("\nGreedy solution based on absolute value\n\n");
    chosenC = value_c;
    greedy(E);

    printf("\nGreedy solution based on relative value\n\n");
    chosenC = relative_value_c;
    greedy(E);

    printf("\nGreedy solution based on medium value\n\n");
    chosenC = medium_value_c;
    greedy(E);

    return 0;
}

elements_t *readElements(){
    FILE *fp = fopen(FILENAME, "r");
    if(fp == NULL) return NULL;
    elements_t *E = malloc(1*sizeof(*E));
    if(E == NULL) return NULL;
    fscanf(fp, "%d", &E->nE);
    E->elements = calloc(E->nE, sizeof(element_t));
    if(E->elements == NULL) return NULL;
    for(int i=0; i<E->nE; i++){
        fscanf(fp, "%s %d %d %d %d %d %f %d\n", E->elements[i].name, &E->elements[i].type, &E->elements[i].in, &E->elements[i].out, &E->elements[i].prev, &E->elements[i].final, &E->elements[i].val, &E->elements[i].difficulty);
        E->elements[i].relVal = E->elements[i].val / (float) E->elements[i].difficulty;
    }
    return E;
}

void greedy(elements_t *E){
    int bonus = -1;
    float val = 0.0;
    int difficulty = 0;
    diagonals_t program;
    int acro = 0;                                                               // acro = 1 [fwd], acro = 2 [bwd], acro = 3 [seq]
    int valid = 1;

    for(int i=0; valid && i<DIAG; i++){                                         // each iteration builds a diagonal
        acro++;                                                                 // assumes first element acro
        valid &= completeDiag(&program.diagonals[i], E, difficulty, acro);      // checks its validity
        if(valid){                                                              // if valid solution, updates values
            difficulty += program.diagonals[i].difficulty;
            val += program.diagonals[i].val;
        }
    }
    if(valid){
        bonus = checkBonus(&program, E);
        if(bonus >= 0)
            val += 0.5 * program.diagonals[bonus].val;
        printf("TOT = %f\n\n", val);                                             // prints solution
        for(int i=0; i<DIAG; i++){                                               // iterates over the diagonals of the program
            printf("DIAG %d > %.3f %s \n", i+1, program.diagonals[i].val, ((bonus == i) ? "BONUS" : ""));
            for(int j=0; j<program.diagonals[i].nE; j++)                         // iterates over the elements chosen for each diagonal
                printf("%s ", E->elements[program.diagonals[i].indexE[j]].name);
            printf("\n\n");
        }
    }
    else printf("Program not valid!\n");
}

int completeDiag(diagonal_t *d, elements_t *E, int diff0, int acro){
    int next, diff = 0, dir = 1;
    d->nE = 0;
    d->val = 0.0;
    for(int i=0; i<E->nE; i++)
        E->elements[i].taken = 0;
    for(int i=0; i<MAX; i++){                                                    // searches for 5 elements
        next = nextBest(E, i==0, dir, diff, diff + diff0, acro, 0);              // assumes element acro=1 and inability to change direction
        if(acro && next<0)                                                       // if it fails
            next = nextBest(E, i==0, dir, diff, diff + diff0, 0, 1);             // retry assuming acro = 0 and the ability to change direction
        if(next < 0)                                                             // if it still fails, break out of the cycle
            break;
        dir = E->elements[next].out;                                             // updates values
        diff += E->elements[next].difficulty;
        d->indexE[i] = next;                                                     // stores index of valid element
        d->nE++;                                                                 // inceases counter
        d->val += E->elements[next].val;
        acro = verifyAcro(d, E, i, acro);
    }
    d->difficulty = diff;
    return(d->nE > 0 && acro == 0);                                              // a solution is found
}

int nextBest(elements_t *E, int first, int dir, int diff, int diffTot, int acro, int changeDir){
    int bestIndex = -1;
    for(int i=0; i<E->nE; i++){
        if(!E->elements[i].taken && (E->elements[i].in == dir) && !(first && E->elements[i].prev) && (diff + E->elements[i].difficulty <= DD) && (diffTot + E->elements[i].difficulty <= DP))
            if(acro == 0 || E->elements[i].type == acro || (acro == 3 && E->elements[i].type > 0)){
                if(changeDir && E->elements[i].out != dir)
                    return i;                                                    // immediately returns the index
                if(bestIndex < 0 || bestElement(&(E->elements[i]), &(E->elements[bestIndex])))
                    bestIndex = i;                                               // stores index
            }
    }
    if(bestIndex >= 0)
        E->elements[bestIndex].taken = 1;                                        // updates flag
    return bestIndex;
}

int bestElement(element_t *e1, element_t *e2){
    switch(chosenC){
        case value_c:
            return e1->val > e2->val;
        case relative_value_c:
            return e1->relVal > e2->relVal;
        case medium_value_c:
            return abs(e1->val - MEDVAL) < abs(e2->val - MEDVAL);                // chooses element closer to the medium value
        default:
            return 0;
    }
}

int verifyAcro(diagonal_t *d, elements_t *E, int i, int acro){
    int curr = d->indexE[i];
    int prev = d->indexE[i-1];
    if((acro == 1 || acro == 2) && E->elements[curr].type == acro)
        return 0;                                                                // requirement met
    else if(acro == 3 && i>0 && E->elements[curr].type != 0)                     // two consecutives types !=0 equal to sequence
        if(E->elements[prev].type != 0)
            return 0;                                                            // requirement met
    return acro;                                                                 // else return acro != 0, requirement not met
}

int checkBonus(diagonals_t *program, elements_t *E){
    int bonus = -1, j;
    for(int i=0; i<DIAG; i++){
        j = program->diagonals[i].nE-1;                                          // gets index for the last element
        if(E->elements[j].type > 0 && E->elements[program->diagonals[i].indexE[j]].difficulty >= 8)
            if(bonus == -1 || (program->diagonals[i].val > program->diagonals[bonus].val))
                bonus = i;
    }
    return bonus;
}