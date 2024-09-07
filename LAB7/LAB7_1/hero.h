#ifndef HERO_H_DEFINED
#define HERO_H_DEFINED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "equipArray.h"

typedef struct{
    char code[LEN];
    char name[LEN];
    char role[LEN];
    equipArray_t equip;
    stat_t h_stat, eq_stat;
} hero_t;

int readHero(FILE *fp, hero_t *hero);
void printHero(FILE *fp, hero_t *hero, invArray_t invArray);
void updateEquipHero(hero_t *hero, invArray_t invArray);
void freeHero(hero_t *hero);

#endif