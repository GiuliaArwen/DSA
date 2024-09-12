#ifndef DATETIME_H_DEFINED
#define DATETIME_H_DEFINED
#define DATELEN 13
#define TIMELEN 6

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int y, m, d;
} Date;

typedef struct{
    int h, m;
} Time;

Date    DATEload(char *s);
Time    TIMEload(char *s);
int     DATEcmp(Date d1, Date d2);
void    DATEstore(FILE *fp, Date d);
void    TIMEstore(FILE *fp, Time t);

#endif