#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include "inv.h"

int readStat(FILE *fp, stat_t *stat){
    if(stat == NULL) return 0;
    return (fscanf(fp, "%d %d %d %d %d %d", &stat->hp, &stat->mp, &stat->atk, &stat->def, &stat->mag, &stat->spr) == 6);
}

void readInv(FILE *fp, inv_t *inv){
    char tmpName[LEN], tmpType[LEN];
    if(inv == NULL) return;
    fscanf(fp, "%s %s", tmpName, tmpType);
    inv->name = strdup(tmpName);
    inv->type = strdup(tmpType);
    readStat(fp, &(inv->stat));
}

tabInv_t *readTabInv(char *filename){
    tabInv_t *tabInv = calloc(1, sizeof(tabInv_t));
    FILE *fp = fopen(filename, "r");
    if(fp == NULL) return NULL;
    fscanf(fp, "%d", &tabInv->nInv);
    tabInv->invArr = calloc(tabInv->nInv, sizeof(inv_t));
    for(int i=0; i<tabInv->nInv; i++)
        readInv(fp, &(tabInv->invArr[i]));
    fclose(fp);
    return tabInv;
}

void printStat(FILE *fp, stat_t s, int limit){
    if(limit) fprintf(fp, "HP = %d, MP = %d, ATK = %d, DEF = %d, MAG = %d, SPR = %d\n", (s.hp>0?s.hp:MIN), (s.mp>0?s.mp:MIN), (s.atk>0?s.atk:MIN), (s.def>0?s.def:MIN), (s.mag>0?s.mag:MIN), (s.spr>0?s.spr:MIN));
    else fprintf(fp, "HP = %d, MP = %d, ATK = %d, DEF = %d, MAG = %d, SPR = %d\n", s.hp, s.mp, s.atk, s.def, s.mag, s.spr);
}

void printInv(FILE *fp, inv_t inv){
    fprintf(fp, "%s [%s]\n\t", inv.name, inv.type);
    printStat(fp, inv.stat, 0);
}

void printTabInv(FILE *fp, tabInv_t *tabInv){
    if(tabInv == NULL) return;
    for(int i=0; i<tabInv->nInv; i++)
        printInv(fp, tabInv->invArr[i]);
}