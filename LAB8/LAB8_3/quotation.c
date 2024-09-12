#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "quotation.h"

static float average(Quotation q){
    if(q.n != 0) return q.val / (float) q.n;
    return -1.0;
}

void QUOTATIONstore(FILE *fp, Quotation q){
    DATEstore(fp, q.date);
    printf(" %.2f\n", QUOTATIONgetVal(q));
}

float QUOTATIONgetVal(Quotation q){
    return average(q);
}

Date QUOTATIONgetDate(Quotation q){
    Date d = {-1, -1, -1};
    if(QUOTATIONcheckNull(q)) return d;
    return q.date;
}

int QUOTATIONcheckNull(Quotation q){
    return (q.n == 0);
}

Quotation QUOTATIONsetNull(){
    Quotation q = {{-1,-1,-1}, 0.0, 0};
    return q;
}