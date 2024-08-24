#include <stdio.h>
#include <stdlib.h>
#define FILENAME "graph.txt"

typedef struct{
    int u;
    int v;
} edge_t;

edge_t *readFile(char *filename, int *N, int *E);
int powerset(int start, int pos, int *sol, int k, edge_t *edge, int N, int E, int cnt);
int check(int *sol, int k, edge_t *edge, int E);

int main(void){
    int N, E, *sol, cnt, tot=0;
    edge_t *edge = readFile(FILENAME, &N, &E);
    sol = calloc(N, sizeof(int));

    printf("Vertex cover\n");
    for(int k=1; k <= N; k++){
        printf("> Dimension: %d\n", k);
        cnt = powerset(0, 0, sol, k, edge, N, E, 0);
        if(cnt == 0) printf("No solution found!\n");
        tot += cnt;
    }
    printf("%d solutions found!", tot);

    free(sol);
    free(edge);
    return 0;
}

edge_t *readFile(char *filename, int *N, int *E){
    FILE *fp = fopen(filename, "r");
    if(fp == NULL) exit(-1);
    
    fscanf(fp, "%d %d", N, E);
    edge_t *edge = (edge_t *) calloc(*E, sizeof(edge_t));
    if(edge == NULL) exit(-1);

    for(int i=0; i<*E; i++) fscanf(fp, "%d %d", &edge[i].u, &edge[i].v);
    return edge;
}

int powerset(int start, int pos, int *sol, int k, edge_t *edge, int N, int E, int cnt){
    if(pos >= k){
        if(check(sol, k, edge, E)){
            for(int i=0; i<k; i++) printf("%d ", sol[i]);
            printf("\n");
            return cnt+1;
        }
        return cnt;
    }
    for(int i=start; i<N; i++){
        sol[pos] = i;
        cnt = powerset(i+1, pos+1, sol, k, edge, N, E, cnt);
    }
    return cnt;
}

int check(int *sol, int k, edge_t *edge, int E){
    int Ecnt=0;
    int *Echeck = calloc(E, sizeof(int));

    for(int i=0; i<k; i++){
        for(int j=0; j<E; j++){
            if(edge[j].u == sol[i] || edge[j].v == sol[i]){
                if(Echeck[j] == 0) Ecnt++;
                Echeck[j] = 1;
            }
        }
    }
    free(Echeck);
    return(Ecnt == E);
}