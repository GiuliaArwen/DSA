#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "heroList.h"
#include "invArray.h"
#include "hero.h"

#define FILENAME1 "heroes.txt"
#define FILENAME2 "inventory.txt"
#define OPT 7

void printMenu(char *options[], int *action){
    printf("\nMENU\n");
    for(int i=0; i<OPT; i++)
        printf("%2d > %s\n", i, options[i]);
    scanf(" %d", action);
}

int main(void){
    char *options[] = {
        "Exit",
        "Print heroes",
        "Print inventory",
        "Search hero",
        "Add hero",
        "Delete hero",
        "Modify equipment"
    };
    char inputCode[LEN];
    int action, proceed = 1;
    heroList_t heroList = initHeroList();
    invArray_t invArray = initInvArray();
    hero_t *hero;

    readHeroList(FILENAME1, heroList);
    readInvArray(FILENAME2, invArray);

    do{
        printMenu(options, &action);
        switch(action){
            case 0:
                proceed = 0;
                break;
            case 1:
                printHeroList(stdout, heroList, invArray);
                break;
            case 2:
                printInvArray(stdout, invArray);
                break;
            case 3:
                printf("Enter hero's code to search: ");
                scanf("%s", inputCode);
                hero = searchHeroListByCode(heroList, inputCode);
                if(hero != NULL) printHero(stdout, hero, invArray);
                break;
            case 4:
                addHeroList(heroList);
                break;
            case 5:
                printf("Enter hero's code to delete: ");
                scanf("%s", inputCode);
                deleteHeroList(heroList, inputCode);
                break;
            case 6:
                printf("Enter hero's code: ");
                scanf("%s", inputCode);
                hero = searchHeroListByCode(heroList, inputCode);
                if(hero != NULL) updateEquipHero(hero, invArray);
                break;
            default:
                printf("Entry not valid!\n");
                break;
        }
    } while (proceed);

    freeHeroList(heroList);
    return 0;
}