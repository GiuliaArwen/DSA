#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "titles.h"

typedef struct node_s *link;
typedef struct node_s{
    Title t;
    link next;
} node;

struct LISTtitles_s{
    link head;
    int count;
};

static link NEW(Title t, link next);

int LISTtitlesEmpty(LISTtitles titles){
    if(titles == NULL || titles->head == NULL || titles->count == 0) return 1;
    return 0;
}

Title LISTtitlesSearch(LISTtitles titles, char *code){
    int cmp;
    link x;
    if(LISTtitlesEmpty(titles)) return NULL;
    for(x = titles->head; x != NULL; x = x->next){
        cmp = TITLEcmp((TitleKey) code, TITLEgetKey(x->t));
        if(cmp == 0) return x->t;
        if(cmp < 0) return NULL; 
    }
    return NULL;
}

void LISTtitlesInsert(LISTtitles titles, Title t){
    link x, p;
    if(titles == NULL) return;
    if(titles->head == NULL)
        titles->head = NEW(t, NULL);
    else if(TITLEcmp(TITLEgetKey(t), TITLEgetKey(titles->head->t)) < 0)
        titles->head = NEW(t, titles->head);
    else{
        for(x=titles->head->next, p=titles->head; x != NULL && TITLEcmp(TITLEgetKey(t), TITLEgetKey(x->t)) > 0; p=x, x=x->next);
        p->next = NEW(t, x);
    }
    titles->count++;
}

static link NEW(Title t, link next){
    link x = calloc(1, sizeof(*x));
    x->t = t;
    x->next = next;
    return x;
}

LISTtitles LISTtitlesInit(){
    LISTtitles titles = calloc(1, sizeof(*titles));
    return titles;
}

void LISTtitlesStore(FILE *fp, LISTtitles titles){
    link x;
    if(LISTtitlesEmpty(titles)) return;
    for(x = titles->head; x != NULL; x = x->next)
        TITLEstore(fp, x->t);
}