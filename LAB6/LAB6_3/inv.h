#ifndef INV_H_DEFINED
#define INV_H_DEFINED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define LEN 100
#define MIN 1

typedef struct{
    int hp, mp, atk, def, mag, spr;
} stat_t;

typedef struct{
    char *name, *type;
    stat_t stat;
} inv_t;

typedef struct{
    int nInv;
    inv_t *invArr;
} tabInv_t;

int readStat(FILE *fp, stat_t *stat);
void printStat(FILE *fp, stat_t s, int limit);
void readInv(FILE *fp, inv_t *inv);
void printInv(FILE *fp, inv_t inv);
tabInv_t *readTabInv(char *filename);
void printTabInv(FILE *fp, tabInv_t *tabInv);

#endif