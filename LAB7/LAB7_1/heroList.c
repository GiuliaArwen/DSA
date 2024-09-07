#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "heroList.h"

typedef struct nodeHero_s *linkHero;
typedef struct nodeHero_s{
    hero_t hero;
    linkHero next;
} nodeHero_t;

typedef struct heroList_s{
    linkHero head, tail;
    int nHero;
};

static linkHero newNodeHero(hero_t hero, linkHero next){
    linkHero new = calloc(1, sizeof(*new));
    if(new == NULL) return NULL;
    new->hero = hero;
    new->next = next;
    return new;
}

heroList_t initHeroList(){
    heroList_t heroList = calloc(1, sizeof(*heroList));
    return heroList;
}

void readHeroList(char *filename, heroList_t heroList){
    hero_t hero;
    FILE *fp = fopen(filename, "r");
    if(fp == NULL) return;
    if(heroList == NULL) return;

    while((readHero(fp, &hero))!=0){
        insertHeroList(heroList, hero);
        heroList->nHero++;
    }
    fclose(fp);
}

void printHeroList(FILE *fp, heroList_t heroList, invArray_t invArray){
    linkHero x = NULL;
    if(heroList == NULL) return;
    for(x = heroList->head; x != NULL; x = x->next)
        printHero(fp, &x->hero, invArray);
}

void insertHeroList(heroList_t heroList, hero_t hero){
    linkHero new;
    new = newNodeHero(hero, NULL);
    if(new == NULL || heroList == NULL) return;
    if(heroList->head == NULL)
        heroList->head = heroList->tail = new;
    else{
        heroList->tail->next = new;
        heroList->tail = new;
    }
}

void addHeroList(heroList_t heroList){
    hero_t hero;
    printf("Code\t Name\t Role\t HP\t MP\t ATK\t DEF\t MAG\t SPR: ");
    if(readHero(stdin, &hero) != 0){
        if(searchHeroListByCode(heroList, hero.code) == NULL)
            insertHeroList(heroList, hero);
    }
}

void deleteHeroList(heroList_t heroList, char *code){
    linkHero x, p;
    for(x = heroList->head, p = NULL; x != NULL; p = x, x = x->next)
        if(strcmp(x->hero.code, code) == 0){
            if(x == heroList->head && x == heroList->tail)
                heroList->head = heroList->tail = NULL;
            else if(x == heroList->head)
                heroList->head = x->next;
            else if(x == heroList->tail){
                heroList->tail = p;
                p->next = NULL;
            }
            else p->next = x->next;
            freeHero(&x->hero);
            free(x);
            break;
        }
}

void freeHeroList(heroList_t heroList){
    linkHero x, next;
    for(x = heroList->head; x != NULL; x = x->next){
        next = x->next;
        freeHero(&x->hero);
        free(x);
    }
}

hero_t *searchHeroListByCode(heroList_t heroList, char *code){
    linkHero x = NULL;
    if(heroList == NULL) return NULL;
    for(x = heroList->head; x != NULL; x = x->next)
        if(strcmp(x->hero.code, code) == 0)
            return(&x->hero);
    return NULL;
}