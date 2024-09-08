#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Item.h"

Item ITEMload(char name1[], char name2[]){
    Item x;
    strcpy(x.computer, name1);
    strcpy(x.network, name2);
    return x;
}

void ITEMstore(Item val){
    printf("Computer: %s\tNetwork: %s\n", val.computer, val.network);
}

Item ITEMsetNull(){
    Item val = {"",""};
    return val;
}

Key KEYget(Item *val){
    return val->computer;
}

void KEYscan(Key node){
    scanf("%s", node);
}

int KEYcmp(Key k1, Key k2){
    return strcmp(k1, k2);
}