#ifndef BSTQUOTATIONS_H_DEFINED
#define BSTQUOTATIONS_H_DEFINED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "datetime.h"
#include "quotation.h"

#define S 2.0

typedef struct BSTquotations_s *BSTquotations;

int             BSTquotationsEmpty(BSTquotations bst);
Quotation       BSTquotationsSearch(BSTquotations bst, Date d);
void            BSTquotationsInsert(BSTquotations bst, Date d, float val, int n);
void            BSTquotationsRange(BSTquotations bst, Date d1, Date d2, float *f1, float *f2);
BSTquotations   BSTquotationsInit();
void            BSTquotationsStore(FILE *fp, BSTquotations bst);
void            BSTquotationsBalance(BSTquotations bst);
void            BSTgetMinMax(BSTquotations bst, float *min, float *max);

#endif