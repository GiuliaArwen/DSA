#ifndef HEROLIST_H_DEFINED
#define HEROLIST_H_DEFINED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hero.h"

typedef struct heroList_s *heroList_t;

heroList_t initHeroList();
void readHeroList(char *filename, heroList_t heroList);
void printHeroList(FILE *fp, heroList_t heroList, invArray_t invArray);
void insertHeroList(heroList_t heroList, hero_t hero);
void addHeroList(heroList_t heroList);
void deleteHeroList(heroList_t heroList, char *code);
void freeHeroList(heroList_t heroList);
hero_t *searchHeroListByCode(heroList_t heroList, char *code);

#endif