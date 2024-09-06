#include <stdio.h>
#include <stdlib.h>

#include "hero.h"

int readHero(FILE *fp, hero_t *hero){
    char tmpCode[LEN], tmpName[LEN], tmpRole[LEN];
    if(hero == NULL) return 0;
    if(fscanf(fp, "%s %s %s", tmpCode, tmpName, tmpRole) == 3){
        hero->code = strdup(tmpCode);
        hero->name = strdup(tmpName);
        hero->role = strdup(tmpRole);
        hero->equip = calloc(1, sizeof(equip_t));
        hero->equip->equipArr = calloc(EQUIP_SLOT, sizeof(inv_t*));
        if(!readStat(fp, &(hero->h_stat))) return 0;
        hero->eq_stat = hero->h_stat;
        return 1;
    }
    return 0;   
}

void printHero(FILE *fp, hero_t hero){
    fprintf(fp, "%s, <> %s [%s]\n\t BASE: ", hero.code, hero.name, hero.role);
    printStat(fp, hero.h_stat, 1);
    if(hero.equip->used > 0){
        fprintf(fp, "\t +EQ: ");
        printStat(fp, hero.eq_stat, 1);
        for(int i=0; i<EQUIP_SLOT; i++)
            if(hero.equip->equipArr[i] != NULL)
                printInv(fp, *(hero.equip->equipArr[i]));
    }
}

tabHero_t *readTabHero(char *filename){
    tabHero_t *tabHero = calloc(1, sizeof(tabHero_t));
    hero_t hero;
    FILE *fp = fopen(filename, "r");
    if(fp == NULL) return NULL;
    while(readHero(fp, &hero)!=0){
        insertNode(tabHero, hero);
        tabHero->nHero++;
    }
    fclose(fp);
    return tabHero;
}

void printTabHero(FILE *fp, tabHero_t *tabHero){
    linkHero x = NULL;
    if(tabHero == NULL) return;
    for(x = tabHero->head; x != NULL; x = x->next)
        printHero(fp, x->hero);
}

linkHero newNodeHero(hero_t hero, linkHero next){
    linkHero new = calloc(1, sizeof(*new)); //???
    if(new == NULL) return NULL;
    new->hero = hero;
    new->next = next;
    return new;
}

void insertNode(tabHero_t *tabHero, hero_t hero){
    linkHero new;
    new = newNodeHero(hero, NULL);
    if(new == NULL || tabHero == NULL) return;
    if(tabHero->head == NULL) tabHero->head = tabHero->tail = new;
    else{
        tabHero->tail = new;
        tabHero->tail->next = new;
    }
}

void addHero(tabHero_t *tabHero){
    hero_t hero;
    printf("Code\t Name\t Class\t HP\t MP\t ATK\t DEF\t MAG\t SPR: ");
    if(readHero(stdin, &hero))
        if(searchCodeRef(tabHero, hero.code, &hero)==0)
            insertNode(tabHero, hero);
}

void deleteHero(tabHero_t *tabHero, char *code){
    linkHero x, p;
    for(x = tabHero->head, p = NULL; x != NULL; p = x, x = x->next)
        if(strcmp(x->hero.code, code) == 0){
            if(x == tabHero->head && x == tabHero->tail)
                tabHero->head = tabHero->tail = NULL;
            else if(x == tabHero->head)
                tabHero->head = x->next;
            else if(x == tabHero->tail){
                tabHero->tail = p;
                p->next = NULL;
            }
            else p->next = x->next;
            freeHero(x->hero);
            free(x);
            break;
        }
}

void modifyEquip(equip_t *equip, tabInv_t *tabInv){
    int action = -1;
    char equipInput[LEN];

    printf("Which action do you want to take?\n");
    if(equip->used = 0)
        printf("\t 0 - Removal\n");
    if(equip->used < EQUIP_SLOT)
        printf("\t 1 - Addition\n");
    scanf("%d", &action);

    if(action == 0 && equip->used <= 0) return;
    if(action == 1 && equip->used >= EQUIP_SLOT) return;
    if(action != 0 && action != 1) return;

    printf("Enter name of the equipment: ");
    scanf("%s", equipInput);
    switch(action){
        case 0:
            for(int i=0; i<EQUIP_SLOT; i++){
                if(equip->equipArr[i] == NULL) continue;
                if(strcmp(equip->equipArr[i]->name, equipInput) == 0){
                    equip->used--;
                    equip->equipArr[i] == NULL;
                    break;
                }
            }
            break;
        case 1:
            for(int i=0; i<tabInv->nInv; i++){
                if(strcmp(tabInv->invArr[i], equipInput) == 0){
                    for(int j=0; j<EQUIP_SLOT; j++)
                        if(equip->equipArr[j] == NULL){
                            equip->equipArr[j] = &(tabInv->invArr[i]);
                            equip->used++;
                            break;
                        }
                    break;
                }
            }
            break;
        default:
            break;
    }
}

void updateHeroStatEquip(hero_t *hero){
    hero->eq_stat = hero->h_stat;
    for(int i=0; i<EQUIP_SLOT; i++){
        if(hero->equip->equipArr[i] == NULL) continue;
        hero->eq_stat.hp += hero->equip->equipArr[i]->stat.hp;
        hero->eq_stat.mp += hero->equip->equipArr[i]->stat.mp;
        hero->eq_stat.atk += hero->equip->equipArr[i]->stat.atk;
        hero->eq_stat.def += hero->equip->equipArr[i]->stat.def;
        hero->eq_stat.mag += hero->equip->equipArr[i]->stat.mag;
        hero->eq_stat.spr += hero->equip->equipArr[i]->stat.spr;
    }
}

void updateHeroEquip(hero_t *hero, tabInv_t *tabInv){
    modifyEquip(hero->equip, tabInv);
    updateHeroStatEquip(hero);
}

int searchCode(tabHero_t *tabHero, char *code, hero_t *hero){
    linkHero x = NULL;
    if(tabHero == NULL) return 0;
    for(x = tabHero->head; x != NULL; x = x->next)
        if(strcmp(x->hero.code, code) == 0){
            *hero = x->hero;
            return 1;
        }
    return 0;
}

int searchCodeRef(tabHero_t *tabHero, char *code, hero_t **hero){
    linkHero x = NULL;
    if(tabHero == NULL) return 0;
    for(x = tabHero->head; x != NULL; x = x->next)
        if(strcmp(x->hero.code, code) == 0){
            *hero = &(x->hero);
            return 1;
        }
    return 0;
}

void freeEquip(equip_t *equip){
    if(equip == NULL) return;
    free(equip->equipArr);
    free(equip);
}

void freeHero(hero_t hero){
    if(hero.code) free(hero.code);
    if(hero.name) free(hero.name);
    if(hero.role) free(hero.role);
    if(hero.equip) freeEquip(hero.equip);
}