#ifndef QUOTATION_H_DEFINED
#define QUOTATION_H_DEFINED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "datetime.h"

typedef struct{
    Date date;
    float val;
    int n;
} Quotation;

void        QUOTATIONstore(FILE *fp, Quotation q);
float       QUOTATIONgetVal(Quotation q);
Date        QUOTATIONgetDate(Quotation q);
Quotation   QUOTATIONnew(Date d);
int         QUOTATIONcheckNull(Quotation q);
Quotation   QUOTATIONsetNull();

#endif