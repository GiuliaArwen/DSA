#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "datetime.h"

Date DATEload(char *sd){
    Date d;
    sscanf(sd, "%d/%d/%d", &d.y, &d.m, &d.d);
    return d;
}

Time TIMEload(char *st){
    Time t;
    sscanf(st, "%d:%d", &t.h, &t.m);
    return t;
}

int DATEcmp(Date d1, Date d2){
    if(d1.y != d2.y) return(d1.y-d2.y);
    if(d1.m != d2.m) return(d1.m-d2.m);
    if(d1.d != d2.d) return(d1.d-d2.d);
    return 0;
}

void DATEstore(FILE *fp, Date d){
    fprintf(fp, "%d/%02d/%02d", d.y, d.m, d.d);
}

void TIMEstore(FILE *fp, Time t){
    fprintf(fp, "%02d:%02d", t.h, t.m);
}