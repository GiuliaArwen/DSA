#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hero.h"

static void updateStatEquipHero(hero_t *hero, invArray_t invArray){
    int index;
    inv_t *inv;
    stat_t statEq;
    hero->eq_stat = hero->h_stat;
    for(int i=0; i<SLOT; i++){
        index = getEquipArrayByIndex(hero->equip, i);
        if(index == -1) continue;
        inv = getInvArrayByIndex(invArray, index);
        if(inv == NULL) continue;
        statEq = getStatInv(inv);
        hero->eq_stat = addStat(hero->eq_stat, statEq);
    }
}

int readHero(FILE *fp, hero_t *hero){
    if(fscanf(fp, "%s %s %s", hero->code, hero->name, hero->role) < 3) return 0;
    readStat(fp, &(hero->h_stat));
    hero->eq_stat = hero->h_stat;
    hero->equip = initEquipArray();
    return 1;
}

void printHero(FILE *fp, hero_t *hero, invArray_t invArray){
    if(hero == NULL) return;
    fprintf(fp, "%s, %s [%s]\n\tBASE: ", hero->code, hero->name, hero->role);
    printStat(fp, &hero->h_stat, 1);
    if(usedEquipArray(hero->equip)>0){
        fprintf(fp, "\t +EQ: ");
        printStat(fp, &hero->eq_stat, 1);
        if(hero->equip != NULL)
            printEquipArray(fp, hero->equip, invArray);
    }
}

void updateEquipHero(hero_t *hero, invArray_t invArray){
    updateEquipArray(hero->equip, invArray);
    updateStatEquipHero(hero, invArray);
}

void freeHero(hero_t *hero){
    if(hero->equip != NULL) freeEquipArray(hero->equip);
    hero->equip = NULL;
}