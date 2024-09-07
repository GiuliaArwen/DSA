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
    char name[LEN];
    char type[LEN];
    stat_t stat;
} inv_t;

void readStat(FILE *fp, stat_t *stat);
void printStat(FILE *fp, stat_t *stat, int limit);
void readInv(FILE *fp, inv_t *inv);
void printInv(FILE *fp, inv_t *inv);
stat_t addStat(stat_t stat1, stat_t stat2);
stat_t getStatInv(inv_t *inv);

#endif