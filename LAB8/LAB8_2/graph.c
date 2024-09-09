#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "item.h"
#include "ST.h"
#include "graph.h"

typedef struct node *link;
struct node{
    int v;
    int wt;
    link next;
};

struct G{
    int **madj;
    link *ladj;
    int V;
    int E;
};

static int **MATinit(int r, int c, int val);
static link NEW(int v, int wt, link next);
static Edge EDGEcreate(int v, int w, int wt);
static void insertE(Graph g, Edge e);
static int adjacent(Graph g, int v, int w);

static link NEW(int v, int wt, link next){
    link x = malloc(sizeof(*x));
    if(x == NULL) return NULL;
    x->v = v;
    x->wt = wt;
    x->next = next;
    return x;
}

static int **MATinit(int r, int c, int val){
    int **mat = malloc(r * sizeof(*mat));
    for(int i=0; i<r; i++) mat[i] = malloc(c * sizeof(int));
    for(int i=0; i<r; i++)
        for(int j=0; j<c; j++)
            mat[i][j] = val;
    return mat;
}

Graph GRAPHinit(int V){
    Graph g = malloc(sizeof(*g));
    if(g == NULL) return NULL;
    g->V = V;
    g->E = 0;
    g->madj = MATinit(V, V, 0);
    g->ladj = NULL;
    return g;
}

void GRAPHfree(Graph g, int listgen){
    link tmp, t;
    for(int v=0; v < g->V; v++) free(g->madj[v]);       // frees matrix
    free(g->madj);
    if(listgen){                                        // eventually frees list
        for(int v=0; v < g->V; v++)
            for(t = g->ladj[v]; t != NULL;){
                tmp = t;
                t = t->next;
                free(tmp);
            }
        free(g->ladj);
    }
    free(g);
}

static Edge EDGEcreate(int v, int w, int wt){
    Edge e;
    e.v = v;
    e.w = w;
    e.wt = wt;
    return e;
}

static void insertE(Graph g, Edge e){
    int v = e.v, w = e.w, wt = e.wt;
    if(g->madj[v][w] == 0) g->E++;                      // if there wasn't an edge, increase counter
    g->madj[v][w] = wt;
    g->madj[w][v] = wt;
}

void GRAPHinsertE(Graph g, int v, int w, int wt){
    insertE(g, EDGEcreate(v, w, wt));
}

int GRAPHmat2list(Graph g){
    if(g->ladj != NULL) return 0;                       // there already is a list
    if(g->madj == NULL) return -1;
    g->ladj = calloc(g->V, sizeof(link));
    if(g->ladj == NULL) return -1;

    for(int v; v < g->V; v++)
        for(int w; w < g->V; w++)
            if(g->madj){
                g->ladj[v] = NEW(w, g->madj[v][w], g->ladj[v]);
                g->ladj[w] = NEW(v, g->madj[v][w], g->ladj[w]);
            }
    return 0;
}

void GRAPHstore(Graph g, ST st){
    if(g == NULL || st == NULL) return;
    if(g->madj == NULL) return;

    for(int i=0; i<g->V; i++){
        STdisplayByIndex(st, i);
        printf("\n");
        for(int j=0; j<g->V; j++)
            if(g->madj[i][j]){
                printf("\t\t");
                STdisplayByIndex(st, j);
                printf(" ");
            }
        printf("\n");
    }
}

static int adjacent(Graph g, int v, int w){
    if(g == NULL) return 0;
    if(g->madj == NULL) return 0;
    return(g->madj[v][w] > 0);                          // meaning there's an edge
}

int GRAPHcheck3clique(Graph g, int x, int y, int z){
    int ok = 1;
    if(g == NULL) return 0;
    if(g->madj == NULL) return 0;

    ok &= adjacent(g, x, y);
    ok &= adjacent(g, x, z);
    ok &= adjacent(g, y, z);

    return ok;
}