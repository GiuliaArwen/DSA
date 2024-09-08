#include <stdio.h>
#include <stdlib.h>

#include "Item.h"
#include "ST.h"

struct symbtab {
    Item *a;
    int maxN;
    int size;
};

ST STinit(int maxN){
    ST st;
    st = malloc(sizeof(*st));
    if(st == NULL) return NULL;
    st->a = malloc(maxN * sizeof(Item));
    if(st->a == NULL){
        free(st);
        return NULL; 
    }
    for(int i=0; i<maxN; i++)
        st->a[i] = ITEMsetNull();
    st->maxN = maxN;
    st->size = 0;
    return st;
}

void STfree(ST st){
    free(st->a);
    free(st);
}

int STcount(ST st){
    return st->size;
}

void STinsert(ST st, Item x){
    int i = st->size++;
    if(st->size > st->maxN){
        st->a = realloc(st->a, (2*st->maxN)*sizeof(Item));
        if(st->a == NULL){
            free(st);
            return NULL;
        }
        st->maxN = 2*st->maxN;
    }
    while((i>0) && KEYcmp(KEYget(&x), KEYget(&st->a[i-1])) == -1){  // iterates until sorted insertion
        st->a[i] = st->a[i-1];                                      // moves item to the right
        i--;                                                        // retrocedes
    }
    st->a[i] = x;                                                   // inserts at right index
}

int searchR(ST st, int l, int r, Key k){
    int m = (l+r)/2;
    if(l>r) return -1;
    if(KEYcmp(k, KEYget(&(st->a[m]))) == 0) return m;
    if(l == r) return -1;
    if(KEYcmp(k, KEYget(&(st->a[m]))) == -1) return searchR(st, l, m-1, k);
    else return searchR(st, m+1, r, k);
}

int STsearch(ST st, Key k){
    return searchR(st, 0, st->size-1, k);
}

void STdisplayByIndex(ST st, int index){
    ITEMstore(st->a[index]);
}