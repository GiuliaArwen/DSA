#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Title.h"

struct Title_s{
    TitleKey code;
    float min, max;
    BSTquotations bstQuotations;
};

static void updateMinMax(Title t);
static void valRange(Title t, Date d1, Date d2, float *f1, float *f2);

void TITLEstore(FILE *fp, Title t){
    fprintf(fp, "%s [MIN = %.2f - MAX = %.2f]\n", (char*) t->code, t->min, t->max);
    BSTquotationsStore(fp, t->bstQuotations);
}

int TITLEcmp(TitleKey k1, TitleKey k2){
    return strcmp((char *) k1, (char *) k2);
}

TitleKey TITLEgetKey(Title t){
    return t->code;
}

Title TITLEnew(char *code){
    Title t = calloc(1, sizeof(*t));
    t->code = (TitleKey) strdup(code);
    t->min = t->max = -1.0;
    t->bstQuotations = BSTquotationsInit();
}

void TITLEinsertTransaction(Title t, Date d, Time h, float val, int n){
    BSTquotationsInsert(t->bstQuotations, d, val, n);
    updateMinMax(t);
    return;
}

static void updateMinMax(Title t){
    float min = -1.0, max = -1.0;
    BSTgetMinMax(t->bstQuotations, &min, &max);
    t->min = min;
    t->max = max;
}

float TITLEabsoluteMin(Title t){
    if(t == NULL) return -1;
    return t->min;
}

float TITLEabsoluteMax(Title t){
    if(t == NULL) return -1;
    return t->max;
}

void TITLErange(Title t, Date d1, Date d2, float *f1, float *f2){
    int cmp = DATEcmp(d1, d2);
    if(cmp < 0) valRange(t, d1, d2, f1, f2);
    else if(cmp > 0) valRange(t, d2, d1, f1, f2);
    else *f1 = *f2 = QUOTATIONgetVal(BSTquotationsSearch(t->bstQuotations, d1));
}

static void valRange(Title t, Date d1, Date d2, float *f1, float *f2){
    BSTquotationsRange(t->bstQuotations, d1, d2, f1, f2);
}

Quotation TITLEgetQuotation(Title t, Date d){
    if(t == NULL) return QUOTATIONsetNull();
    if(t->bstQuotations == NULL) return QUOTATIONsetNull();
    return BSTquotationsSearch(t->bstQuotations, d);
}

void TITLEtreeBalance(Title t){
    if(t == NULL || t->bstQuotations == NULL) return;
    BSTquotationsBalance(t->bstQuotations);
}