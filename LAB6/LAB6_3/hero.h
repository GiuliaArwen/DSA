#ifndef HERO_H_DEFINED
#define HERO_H_DEFINED

#include <stdio.h>
#include <stdlib.h>
#include <string.h> 

#include "inv.h"

#define EQUIP_SLOT 8

typedef struct{
    int used;
    inv_t **equipArr;
} equip_t;

typedef struct{
    char *code;
    char *name;
    char *role;
    equip_t *equip;
    stat_t h_stat, eq_stat;
} hero_t;

typedef struct nodeHero_s* linkHero;
typedef struct nodeHero_s{
    hero_t hero;
    linkHero next;
} nodeHero_t;

typedef struct{
    linkHero head, tail;
    int nHero;
} tabHero_t;

int readHero(FILE *fp, hero_t *hero);
void printHero(FILE *fp, hero_t hero);
tabHero_t *readTabHero(char *filename);
void printTabHero(FILE *fp, tabHero_t *tabHero);
linkHero newNodeHero(hero_t hero, linkHero next);
void insertNode(tabHero_t *tabHero, hero_t hero);
void addHero(tabHero_t *tabHero);
void deleteHero(tabHero_t *tabHero, char *code);
void modifyEquip(equip_t *equip, tabInv_t *tabInv);
void updateHeroStatEquip(hero_t *hero);
void updateHeroEquip(hero_t *hero, tabInv_t *tabInv);
int searchCode(tabHero_t *tabHero, char *code, hero_t *hero);
int searchCodeRef(tabHero_t *tabHero, char *code, hero_t **hero);
void freeEquip(equip_t *equip);
void freeHero(hero_t hero);

#endif