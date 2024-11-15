#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "st.h"

#define MAXL 31

static int sizeSet(int maxN);
static int hash(char *str, int M);

typedef struct htItem_t{
    char key[MAXL];
    int index;
} htItem;

struct hashtable{
    htItem *items;
    int N, M;                               // N is the number of items, while M is the size of the table
};

static int sizeSet(int maxN){               // determines a size which is at least twice maxN
    int primes[16] = {2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53};
    if(maxN < primes[15])
        for(int i=0; i<16; i++)
            if(maxN*2 <= primes[i])
                return primes[i];
    return -1;
}

static int hash(char *str, int M){          // computes hash value
    int h=0, base=127;
    for(; *str != '\0'; str++)
        h = (base*h + *str) % M;
    return h;
}

ST STinit(int maxN){
    ST ht;
    ht = malloc(sizeof(*ht));
    if(ht == NULL) return NULL;
    ht->N = 0;
    ht->M = sizeSet(maxN);
    ht->items = calloc(ht->M, sizeof(htItem));
    if(ht->items == NULL) return NULL;
    for(int i=0; i<ht->M; i++)
        ht->items[i].index = -1;
    return ht;
}

void STinsert(ST ht, char *str, int index){ // linear probing insertion
    int i=hash(str, ht->M);
    while(ht->items[i].index != -1)
        i = (i+1) % ht->M;
    strcpy(ht->items[i].key, str);
    ht->items[i].index = index;
    ht->N++;
}

int STsearch(ST ht, char *key){
    int i=hash(key, ht->M);
    while(ht->items[i].index != -1)
        if(strcmp(key, ht->items[i].key) == 0)
            return ht->items[i].index;
        else
            i = (i+1) % ht-> M;
    return -1;
}

void STfree(ST ht){
    if(ht == NULL) return;
    free(ht->items);
    free(ht);
}