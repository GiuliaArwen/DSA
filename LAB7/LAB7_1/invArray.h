#ifndef INVARRAY_H_DEFINED
#define INVARRAY_H_DEFINED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "inv.h"

typedef struct invArray_s *invArray_t;

invArray_t initInvArray();
void readInvArray(char *filename, invArray_t invArray);
void printInvArray(FILE *fp, invArray_t invArray);
void printInvArrayByIndex(FILE *fp, invArray_t invArray, int index);
inv_t *getInvArrayByIndex(invArray_t invArray, int index);
int searchInvArrayByName(invArray_t invArray, char *name);

#endif