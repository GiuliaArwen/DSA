#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "invArray.h"

struct invArray_s{
    inv_t *inv;
    int nInv;
    int maxInv;
};

invArray_t initInvArray(){
    invArray_t invArray = calloc(1, sizeof(*invArray));
    return invArray;
}

void readInvArray(char *filename, invArray_t invArray){
    FILE *fp = fopen(filename, "r");
    if(invArray == NULL) return;
    if(fp == NULL) return;
    fscanf(fp, "%d", &invArray->nInv);
    invArray->inv = calloc(invArray->nInv, sizeof(inv_t));
    for(int i=0; i<invArray->nInv; i++)
        readInv(fp, &invArray->inv[i]);
    fclose(fp);
    return;
}

void printInvArray(FILE *fp, invArray_t invArray){
    if(invArray == NULL) return;
    for(int i=0; i<invArray->nInv; i++)
        printInv(fp, &invArray->inv[i]);
}

void printInvArrayByIndex(FILE *fp, invArray_t invArray, int index){
    if(invArray == NULL) return;
    printInv(fp, &invArray->inv[index]);
}

inv_t *getInvArrayByIndex(invArray_t invArray, int index){
    if(invArray == NULL) return NULL;
    if(invArray->inv == NULL) return NULL;
    if(index < 0 || index >= invArray->nInv) return NULL;
    return &invArray->inv[index];
}

int searchInvArrayByName(invArray_t invArray, char *name){
    if(invArray == NULL || name == NULL) return -1;
    for(int i=0; i<invArray->nInv; i++)
        if(strcmp(invArray->inv[i].name, name) == 0)
            return i;
    return -1;
}