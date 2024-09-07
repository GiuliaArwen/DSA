#ifndef EQUIPARRAY_H_DEFINED
#define EQUIPARRAY_H_DEFINED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "invArray.h"

#define SLOT 8

typedef struct equipArray_s *equipArray_t;

equipArray_t initEquipArray();
void freeEquipArray(equipArray_t equipArray);
int usedEquipArray(equipArray_t equipArray);
void printEquipArray(FILE *fp, equipArray_t equipArray, invArray_t invArray);
void updateEquipArray(equipArray_t equipArray, invArray_t invArray);
int getEquipArrayByIndex(equipArray_t equipArray, int index);

#endif