#ifndef TITLE_H_DEFINED
#define TITLE_H_DEFINED

#define LEN 256

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "quotations.h"

typedef char *TitleKey;
typedef struct Title_s *Title;

void        TITLEstore(FILE *fp, Title t);
int         TITLEcmp(TitleKey k1, TitleKey k2);
TitleKey    TITLEgetKey(Title t);
Title       TITLEnew(char *code);
void        TITLEinsertTransaction(Title t, Date d, Time h, float val, int n);
float       TITLEabsoluteMin(Title t);
float       TITLEabsoluteMax(Title t);
void        TITLErange(Title t, Date d1, Date d2, float *f1, float *f2);
Quotation   TITLEgetQuotation(Title t, Date d);
void        TITLEtreeBalance(Title t);

#endif