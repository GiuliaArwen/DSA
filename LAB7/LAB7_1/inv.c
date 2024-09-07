#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "inv.h"

void readStat(FILE *fp, stat_t *stat){
    fscanf(fp, "%d %d %d %d %d %d", &stat->hp, &stat->mp, &stat->atk, &stat->def, &stat->mag, &stat->spr);
}

void printStat(FILE *fp, stat_t *stat, int limit){
    if(limit)
        fprintf(fp, "HP = %d, MP = %d, ATK = %d, DEF = %d, MAG = %d, SPR = %d\n", (stat->hp) > 0 ? stat->hp : MIN, (stat->mp) > 0 ? stat->mp : MIN, (stat->atk) > 0 ? stat->atk : MIN, (stat->def) > 0 ? stat->def : MIN, (stat->mag) > 0 ? stat->mag : MIN, (stat->spr) > 0 ? stat->spr : MIN);
    else
        fprintf(fp, "HP = %d, MP = %d, ATK = %d, DEF = %d, MAG = %d, SPR = %d\n", stat->hp, stat->mp, stat->atk, stat->def, stat->mag, stat->spr);
}

void readInv(FILE *fp, inv_t *inv){
    fscanf(fp, "%s %s", inv->name, inv->type);
    readStat(fp, &(inv->stat));
}

void printInv(FILE *fp, inv_t *inv){
    fprintf(fp, "%s [%s]\n\t", inv->name, inv->type);
    printStat(fp, &(inv->stat), 0);
}

stat_t addStat(stat_t stat1, stat_t stat2){
    stat1.hp += stat2.hp;
    stat1.mp += stat2.mp;
    stat1.atk += stat2.atk;
    stat1.def += stat2.def;
    stat1.mag += stat2.mag;
    stat1.spr += stat2.spr;
    return stat1;
}

stat_t getStatInv(inv_t *inv){
    return inv->stat;
}