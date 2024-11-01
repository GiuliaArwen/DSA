#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "st.h"
#include "graph.h"

#define MAXL 31
#define MAXN 10

void generateSetE(Graph g);

int main(int argc, char **argv){
    int V, e=1, *ts;
    Graph g = NULL;
    FILE *fp;

    fp = fopen(argv[1], "r");
    if(fp == NULL) exit(-1);

    g = GRAPHload(fp);
    if(g == NULL) exit(-1);

    GRAPHdfs(g, &e, 0, 0);
    if(!e){
        printf("Original graph is acyclic.\n\n");
        generateSetE(g);
        printf("The resulting DAG is:\n\n");
        GRAPHstore(g, stdout);
        printf("\n");
    }
    else
        printf("Original graph is cyclic.\n\n");
    V = GRAPHgetNumV(g);
    ts = malloc(V*sizeof(int));
    DAGts(g, ts);
    printf("Topological order of the graph is:\n");
    for(int i=0; i<V; i++)
        printf("%s ", GRAPHgetName(g, ts[i]));
    printf("\n\n");
    printf("Maximum distances between vertexes (-1 => unreachable)\n");
    DAGmaxPath(g, ts);
    GRAPHfree(g);
    free(ts);
    return 0;
}

void comb(int pos, int *sol, Graph g, Edge *vE, int n, int k, int start, int *stop, int *bestSol, int *bestWt){
    int e=1, i, solWt;
    if(pos >= k){
        for(i=0; i<k; i++);
            GRAPHremoveE(g, vE[sol[i]]);
        solWt = GRAPHedgesWt(g, vE, sol, k);
        GRAPHdfs(g, &e, 0,  0);
        if(e){
            *stop=1;
            if(solWt > *bestWt){
                *bestWt = solWt;
                for(i=0; i<k; i++)
                    bestSol[i] = sol[i];
            }
        }
        for(i=0; i<k; i++)
            GRAPHinsertE(g, vE[sol[i]]);
        return;
    }
    for(i=start; i<n; i++){
        sol[pos]=i;
        comb(pos+1, sol, g, vE, n, k, i+1, stop, bestSol, bestWt);
    }
}

void generateSetE(Graph g){
    int i, j, E=GRAPHgetNumE(g), V=GRAPHgetNumV(g), stop=0, bestWt=0, *sol;
    Edge *vE = malloc(E*sizeof(Edge));
    int *bestSol = calloc(E, sizeof(int));
    int upper_bound = E - (V-1);
    if(upper_bound <= 0) return;
    printf("\nV=%d, E=%d, maximum number of edges to remove %d\n", V, E, upper_bound);
    GRAPHedges(g, vE);
    for(i=1; i<=upper_bound && !stop; i++){
        sol = malloc(i*sizeof(int));
        printf("Generating set having limit %d\n", i);
        comb(0, sol, g, vE, E, i, 0, &stop, bestSol, &bestWt);
    }
    printf("Edges removed for optimal solution:\n");
    for(j=0; j<i-1; j++)
        EDGEstore(g, vE[bestSol[j]], stdout);
    printf("\n");
    if(stop)
        for(j=0; j<i-1; j++)
            GRAPHremoveE(g, vE[bestSol[j]]);
}