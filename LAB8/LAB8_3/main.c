#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "datetime.h"
#include "titles.h"
#include "quotations.h"

#define FILENAME1 "F1.txt"
#define FILENAME2 "F2.txt"
#define FILENAME3 "F3.txt"
#define OPT 8

void readFile(char *filename, LISTtitles titles);
void printMenu(char *options[], int *action);

int main(void){
    char *options[] = {
        "Exit",
        "Read file",
        "Print titles",
        "Print title",
        "Print quotation",
        "Print quotation's absolute MIN/MAX",
        "Print quotation's range MIN/MAX",
        "Balance"
    };
    LISTtitles titles = LISTtitlesInit();
    Title t;
    Quotation q;
    float f1, f2;
    char strTmp[100], strTmp1[100];
    int action = 0, proceed = 1;

    readFile(FILENAME1, titles);
    readFile(FILENAME2, titles);
    readFile(FILENAME3, titles);

    do{
        printMenu(options, &action);
        switch(action){
            case 0:
                proceed = 0;
                break;
            case 1:
                printf("Enter file name: ");
                scanf("%s", strTmp);
                readFile(strTmp, titles);
                break;
            case 2:
                LISTtitlesStore(stdout, titles);
                break;
            case 3:
                printf("Enter title's code: ");
                scanf("%s", strTmp);
                t = LISTtitlesSearch(titles, strTmp);
                if(t != NULL) TITLEstore(stdout, t);
                break;
            case 4:
                printf("Enter title's code: ");
                scanf("%s", strTmp);
                t = LISTtitlesSearch(titles, strTmp);
                if(t != NULL){
                    printf("Enter date: ");
                    scanf("%s", strTmp);
                    q = TITLEgetQuotation(t, DATEload(strTmp));
                    QUOTATIONstore(stdout, q);
                }
                break;
            case 5:
                printf("Enter title's code: ");
                scanf("%s", strTmp);
                t = LISTtitlesSearch(titles, strTmp);
                if(t != NULL) printf("MIN = %f || MAX = %f\n", TITLEabsoluteMin(t), TITLEabsoluteMax(t));
                break;
            case 6:
                printf("Enter title's code: ");
                scanf("%s", strTmp);
                t = LISTtitlesSearch(titles, strTmp);
                if(t != NULL){
                    printf("Enter first date: ");
                    scanf("%s", strTmp);
                    printf("Enter second date: ");
                    scanf("%s", strTmp1);
                    TITLErange(t, DATEload(strTmp), DATEload(strTmp1), &f1, &f2);
                    printf("MIN = %f || MAX = %f\n", f1, f2);
                }
                break;
            case 7:
                printf("Enter title's code: ");
                scanf("%s", strTmp);
                t = LISTtitlesSearch(titles, strTmp);
                if(t != NULL) TITLEtreeBalance(t);
                break;
            default:
                printf("Invalid option.\n");
                break;
        }

    }while(proceed);

    return 0;
}

void readFile(char *filename, LISTtitles titles){
    char code[LEN], date[DATELEN], time[TIMELEN];
    int nT, nQ = 0, n;
    float val;
    Title t;
    FILE *fp = fopen(filename, "r");
    if(fp == NULL) return;
    fscanf(fp, "%d\n", &nT);
    for(int i=0; i<nT; i++){
        fscanf(fp, "%s %d\n", code, &nQ);
        t = LISTtitlesSearch(titles, code);
        if(t == NULL){
            t = TITLEnew(code);
            LISTtitlesInsert(titles, t);
        }
        for(int j=0; j<nQ; j++){
            fscanf(fp, "%s %s %f %d\n", date, time, &val, &n);
            TITLEinsertTransaction(t, DATEload(date), TIMEload(time), val, n);
        }
    }
    fclose(fp);
    return;
}

void printMenu(char *options[], int *action){
    printf("\nMENU\n");
    for(int i=0; i<OPT; i++)
        printf("%2d > %s\n", i, options[i]);
    scanf(" %d", action);
}