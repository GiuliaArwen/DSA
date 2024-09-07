#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "equipArray.h"

struct equipArray_s{
    int equip[SLOT];
    int used;
};

equipArray_t initEquipArray(){
    equipArray_t equipArray = calloc(1, sizeof(*equipArray));
    if(equipArray != NULL)
        for(int i=0; i<SLOT; i++)
            equipArray->equip[i] = -1;
    return equipArray;
}

void freeEquipArray(equipArray_t equipArray){
    if(equipArray != NULL) free(equipArray);
}

int usedEquipArray(equipArray_t equipArray){
    if(equipArray == NULL) return 0;
    return equipArray->used;
}

void printEquipArray(FILE *fp, equipArray_t equipArray, invArray_t invArray){
    if(equipArray == NULL || invArray == NULL) return;
    if(equipArray->used > 0)
        for(int i=0; i<SLOT; i++)
            if(equipArray->equip[i] != -1)
                printInvArrayByIndex(fp, invArray, equipArray->equip[i]);
}

void updateEquipArray(equipArray_t equipArray, invArray_t invArray){
    int action = -1, eq;
    char inputName[LEN];

    printf("Which action do you want to take?\n");
    if(equipArray->used > 0) printf("\t 0 - Removal\n");
    if(equipArray->used < SLOT) printf("\t 1- Addition\n");
    scanf("%d", &action);
    if(action == 0 && equipArray->used <= 0) return;
    if(action == 1 && equipArray->used >= SLOT) return;
    if(action != 0 && action != 1) return;

    printf("Enter equipment name: ");
    scanf("%s", inputName);
    eq = searchInvArrayByName(invArray, inputName);
    if(eq == -1) return;

    switch(action){
        case 0:
            for(int i=0; i<SLOT; i++){
                if(equipArray->equip[i] == -1) continue;
                if(equipArray->equip[i] == eq){
                    equipArray->used--;
                    equipArray->equip[i] = -1;
                    break;
                }
            }
            break;
        case 1:
            for(int i=0; i<SLOT; i++)
                if(equipArray->equip[i] == -1){
                    equipArray->equip[i] = eq;
                    equipArray->used++;
                }
            break;
        default:
            break;
    }
}

int getEquipArrayByIndex(equipArray_t equipArray, int index){
    if(equipArray == NULL) return -1;
    if(equipArray->equip == NULL) return -1;
    if(index < 0 || index >= SLOT) return -1;
    return equipArray->equip[index];
}