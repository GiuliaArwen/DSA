#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "st.h"
#include "graph.h"

#define NO_EDGE -1

struct G{
    int V, E;
    int **madj;
    ST st;
    Vertex *arr;
};

static int **MATRIXint(int r, int c, int val){
    int i, j, **t;
    t = malloc(r * sizeof(int *));
    if(t == NULL) return NULL;
    for(i=0; i<r; i++){
        t[i] = malloc(c * sizeof(int));
        if(t[i] == NULL) return NULL;
    }
    for(i=0; i<r; i++)
        for(j=0; j<c; j++)
            t[i][j] = val;
    return t;
}

char *GETname(Vertex *v){
    return (v->name);
}

Graph GRAPHinit(int nV){
    Graph g = malloc(sizeof(*g));
    if(g == NULL) return NULL;

    g->V = nV;
    g->E = 0;

    g->madj = MATRIXint(nV, nV, NO_EDGE);
    if(g->madj == NULL) return NULL;

    g->st = STinit(nV);
    if(g->st == NULL) return NULL;

    g->arr = malloc((nV)*sizeof(Vertex));
    if(g->arr == NULL) return NULL;

    return g;
}

Graph GRAPHload(FILE *fp){
    int i, V, wt, id1, id2;
    char src[MAXL], dst[MAXL];
    Graph g;

    fscanf(fp, "%d", &V);
    g = GRAPHinit(V);
    if(g == NULL) return NULL;

    for(i=0; i<V; i++){
        fscanf(fp, "%s", src);
        strcpy(g->arr[i].name, src);
        STinsert(g->st, src, i);
    }

    while(fscanf(fp, "%s %s %d", src, dst, &wt) == 3){
        id1 = STsearch(g->st, src);
        id2 = STsearch(g->st, dst);
        if(id1 != id2 && id1 >= 0 && id2 >= 0)
        GRAPHinsertE(g, EDGEcreate(id1, id2, wt));
    }

    return g;
}

void GRAPHfree(Graph g){
    for(int i=0; i < g->V; i++)
        free(g->madj[i]);
    free(g->madj);
}

Edge EDGEcreate(int v, int w, int wt){
    Edge e;
    e.v = v;
    e.w = w;
    e.wt = wt;
    return e;
}

void EDGEstore(Graph g, Edge e, FILE *fp){
    fprintf(fp, "%s %s %d\n", GETname(&(g->arr[e.v])), GETname(&(g->arr[e.w])), e.wt);
}

void GRAPHinsertE(Graph g, Edge e){
    int v = e.v, w = e.w, wt = e.wt;
    g->madj[v][w] = wt;
    g->E++;
}

void GRAPHstore(Graph g, FILE *fp){
    Edge *e;

    if(g == NULL) return;
    if(g->madj == NULL) return;

    e = malloc(g->E * sizeof(Edge));
    if(e == NULL) return;

    GRAPHedges(g, e);

    fprintf(fp, "%d\n", g->V);
    for(int i=0; i < g->V; i++)
        fprintf(fp, "%s\n", GETname(&(g->arr[i])));
    for(int i=0; i < g->E; i++)
        fprintf(fp, "%s %s %d\n", GETname(&(g->arr[e[i].v])), GETname(&(g->arr[e[i].w])), e[i].wt);
}

int GRAPHgetNumV(Graph g){
    if(g == NULL) return -1;
    return g->V;
}

int GRAPHgetNumE(Graph g){
    if(g == NULL) return -1;
    return g->E;
}

void dfsR(Graph g, int start, int *time, int *pre, int *post, int *isAcyclic){
    pre[start] = (*time)++;
    for(int i=0; i < g->V; i++){
        if(g->madj[start][i] != NO_EDGE){
            if(pre[i] == -1)
                dfsR(g, i, time, pre, post, isAcyclic);
            else
                if(post[i] == -1)
                    *isAcyclic = 0;
        }
    }
    post[start] = (*time)++;
}

void GRAPHdfs(Graph g, int *isAcyclic){
    int *pre, *post, time=0;

    if(g == NULL) return;
    if(g->madj == NULL) return;

    pre = calloc(g->V, sizeof(int));
    post = calloc(g->V, sizeof(int));

    for(int i=0; i < g->V; i++)
        pre[i] = post[i] = -1;
    for(int i=0; i < g->V; i++)
        if(pre[i] == -1)
            dfsR(g, i, &time, pre, post, isAcyclic);
    free(pre);
    free(post);
    return;
}

void GRAPHedges(Graph g, Edge *e){
    int E=0;
    if(g == NULL) return;
    if(g->E <= 0) return;
    for(int v=0; v < g->V; v++)
        for(int w=0; w < g->V; w++)
            if(g->madj[v][w] != NO_EDGE)
                e[E++] = EDGEcreate(v, w, g->madj[v][w]);
}

int GRAPHedgesWt(Graph g, Edge *e, int *subset, int k){
    int totWt = 0;
    if(g == NULL) return 0;
    for(int i=0; i<k; i++)
        totWt += e[subset[i]].wt;
    return totWt;
}

void GRAPHremoveE(Graph g, Edge e){
    int v = e.v, w = e.w;
    if(g->madj[v][w] != NO_EDGE)
        g->E--;
    g->madj[v][w] = NO_EDGE;
}

void TSdfsR(Graph g, int v, int *ts, int *pre, int *time){
    int w;
    pre[v] = 0;
    for(w=0; w < g->V; w++)
        if(g->madj[w][v] != NO_EDGE)
            if(pre[w] == -1)
                TSdfsR(g, w, ts, pre, time);
    ts[(*time)++] = v;
}

void DAGts(Graph g, int *ts){
    int time=0, *pre;
    pre = malloc(g->V*sizeof(int));
    if((pre == NULL) || (ts == NULL)) return;
    for(int i=0; i < g->V; i++){
        pre[i] = -1;
        ts[i] = -1;
    }
    for(int i=0; i < g->V; i++)
        if(pre[i] == -1)
            TSdfsR(g, i, ts, pre, &time);
    free(pre);
}

void DAGmaxPath(Graph g, int *ts){
    int v, w;
    int *dist = malloc(g->V * sizeof(int));

    for(int i=0; i<g->V; i++){
        v = ts[i];
        printf("Start: %s\n", GETname(&(g->arr[v])));
        for(int j=0; j<g->V; j++) dist[j] = -1;
        dist[v] = 0;
        for(int j=i; j<g->V; j++){
            w = ts[j];
            if(dist[w] == -1) continue;
            for(int k=0; k<g->V; k++){
                if(g->madj[w][k] != NO_EDGE)
                    if(dist[k] == -1 || (dist[w] + g->madj[w][k]) > dist[k])
                        dist[k] = dist[w] + g->madj[w][k];
            }
        }
        for(int j=0; j<g->V; j++){
            if(j == v) continue;
            printf("\t -> %s [%d]\n", GETname(&(g->arr[j])), dist[j]);
        }
    }
    free(dist);
    return;
}

char *GRAPHgetName(Graph g, int index){
    if(g == NULL) return NULL;
    if(g->arr == NULL) return NULL;
    return g->arr[index].name;
}