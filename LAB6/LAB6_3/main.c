#include <stdio.h>
#include <stdlib.h>
#include <string.h> 

#include "inv.h"
#include "hero.h"

#define FILENAME1 "inventory.txt"
#define FILENAME2 "heroes.txt"
#define OPT 7

void printMenu(char *options[], int *choice){
    printf("\nMENU\n");
    for(int i=0; i<OPT; i++)
        printf("%2d > %s\n", i, options[i]);
    scanf("%d", choice);
}

int main(void){
    char *options[] = {
        "Exit",
        "Print Hero",
        "Print inventory",
        "Search Hero",
        "Add Hero",
        "Delete Hero",
        "Modify equipment"
    };
    char codeInput[LEN];
    int choice, proceed=1;
    tabInv_t *tabInv;
    tabHero_t *tabHero;
    hero_t hero, *heroP;

    tabInv = readTabInv(FILENAME1);
    if(tabInv == NULL) return 0;
    tabHero = readTabHero(FILENAME2);
    if(tabHero == NULL) return 0;

    do{
        printMenu(options, &choice);
        switch(choice){
            case 0:
                proceed=0;
                break;
            case 1:
                printTabHero(stdout, tabHero);
                break;
            case 2:
                printTabInv(stdout, tabInv);
                break;
            case 3:
                printf("\nEnter hero's code: ");
                scanf("%s", codeInput);
                if(searchCode(tabHero, codeInput, &hero) != 0)
                    printHero(stdout, hero);
                break;
            case 4:
                addHero(tabHero);
                break;
            case 5:
                printf("\nEnter hero's code: ");
                scanf("%s", codeInput);
                deleteHero(tabHero, codeInput);
                break;
            case 6:
                printf("\nEnter hero's code: ");
                scanf("%s", codeInput);
                if(searchCodeRef(tabHero, codeInput, &heroP) != 0)
                    updateHeroEquip(heroP, tabInv);
                break;
            default:
                printf("\nOption not valid!\n");
                break;
        }
    } while(proceed);

    return 0;
}