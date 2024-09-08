#ifndef GRAPH_H_DEFINED
#define GRAPH_H_DEFINED

#define LEN 31

#include "ST.h"

typedef struct edge{
    int v;
    int w;
    int wt;
} Edge;

typedef struct G* Graph;

Graph   GRAPHinit(int V);
void    GRAPHfree(Graph g, int listgen);
void    GRAPHinsertE(Graph g, int v, int w, int wt);
void    GRAPHstore(Graph g, ST st);
int     GRAPHcheck3clique(Graph g, int x, int y, int z);
int     GRAPHmat2list(Graph g);

#endif