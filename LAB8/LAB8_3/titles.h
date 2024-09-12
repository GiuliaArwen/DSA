#ifndef LISTTITLES_H_DEFINED
#define LISTTITLES_H_DEFINED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "title.h"

#define LEN 256

typedef struct LISTtitles_s *LISTtitles;

int         LISTtitlesEmpty(LISTtitles titles);
Title       LISTtitlesSearch(LISTtitles titles, char *code);
void        LISTtitlesInsert(LISTtitles titles, Title t);
LISTtitles  LISTtitlesInit();
void        LISTtitlesStore(FILE *fp, LISTtitles titles);

#endif