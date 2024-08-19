#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ROW 1000
#define MAX_STR 31
#define CMD 9
#define FILE_NAME "../log.txt"

typedef enum {NONE, DATE, CODE, DEPARTURE, ARRIVAL} key_e;

typedef struct{
    int year;
    int month;
    int day;
} date_t;

typedef struct{
    int h;
    int min;
    int sec;
} time_t;

typedef struct{
    char code[MAX_STR];
    char departure[MAX_STR];
    char arrival[MAX_STR];
    char date_str[MAX_STR];
    char timeD_str[MAX_STR];
    char timeA_str[MAX_STR];
    int delay;
    date_t date;
    time_t timeD, timeA;
} entry_t;

typedef struct{
    int n_entries;
    entry_t log[MAX_ROW];
    key_e key;
} table_t;

void readTable(table_t *table);
void printCommands(char *commands[], int *choice);
void printOut(table_t *tab, char *outfile);
void insertionSort(table_t *tab, key_e key);
int compareEntries(entry_t e1, entry_t e2, key_e k);
int compareDates(date_t d1, date_t d2);
int compareTimes(time_t t1, time_t t2);
void printEntry(entry_t e, FILE *fp);
int linearSearchCode(table_t *tab, char *code);
int binarySearchCode(table_t *tab, char *code);
void linearSearchStation(table_t *tab, char *station, int n);
void binarySearchStation(table_t *tab, char *station, int n);

int main(void){
    char *commands[] = {"Exit", 
                        "Print on video", 
                        "Print on file", 
                        "Order by date", 
                        "Order by code", 
                        "Order by departure station",
                        "Order by arrival station",
                        "Search by code",
                        "Search by departure station"};
    int proceed = 1, choice, nChar;
    char outfile[MAX_STR], inputCode[MAX_STR], inputStation[MAX_STR];

    table_t tab;
    readTable(&tab);

    do{
        printCommands(commands, &choice);
        switch(choice){
        case 0:
            proceed = 0;
            break;
        case 1:
            printOut(&tab, stdout);
            break;
        case 2:
            printf("Enter output file name: ");
            scanf("%s", outfile);
            printOut(&tab, outfile);
            break;
        case 3:
            printf("Ordering by date...\n");
            insertionSort(&tab, DATE);
            printOut(&tab, stdout);
            break;
        case 4:
            printf("Ordering by code...\n");
            insertionSort(&tab, CODE);
            printOut(&tab, stdout);
            break;
        case 5:
            printf("Ordering by departure station...\n");
            insertionSort(&tab, DEPARTURE);
            printOut(&tab, stdout);
            break;
        case 6:
            printf("Ordering by arrival station...\n");
            insertionSort(&tab, ARRIVAL);
            printOut(&tab, stdout);
            break;
        case 7:
            printf("Enter route code to search: ");
            scanf("%s", inputCode);
            if(tab.key == CODE) binarySearchCode(&tab, inputCode);                      // binary search on already sorted table
            else linearSearchCode(&tab, inputCode);
            break;
        case 8:
            printf("Enter departure station to search: ");
            scanf("%s%n", inputStation, &nChar);                                        // nChar needed to detect only partial matches
            if(tab.key == DEPARTURE) binarySearchStation(&tab, inputStation, nChar-1);
            else linearSearchStation(&tab, inputStation, nChar-1);
            break;
        default:
            printf("Unavailable choice!");
            break;
        }
    } while(proceed);
    return 0;
}

void readTable(table_t *tab){
    FILE *fp = fopen(FILE_NAME, "r");
    if (fp == NULL) exit (-1);
    
    fscanf(fp, "%d", &tab->n_entries);
    for(int i=0; i<tab->n_entries; i++){
        fscanf(fp, "%s %s %s %s %s %s %d", tab->log[i].code, tab->log[i].departure, tab->log[i].arrival, tab->log[i].date_str, tab->log[i].timeD_str, tab->log[i].timeA_str, &tab->log[i].delay);
        sscanf(tab->log[i].date_str, "%d/%d/%d", &tab->log[i].date.year, &tab->log[i].date.month, &tab->log[i].date.day);
        sscanf(tab->log[i].timeD_str, "%d:%d:%d", &tab->log[i].timeD.h, &tab->log[i].timeD.min, &tab->log[i].timeD.sec);
        sscanf(tab->log[i].timeA_str, "%d:%d:%d", &tab->log[i].timeA.h, &tab->log[i].timeA.min, &tab->log[i].timeA.sec);
    }

    fclose(fp);
}

void printCommands(char *commands[], int *choice){
    printf("\nMENU:\n");
    for(int i=0; i<CMD; i++) printf("%2d\t>\t%s\n", i, commands[i]);
    scanf(" %d", choice);
}

void printOut(table_t *tab, char *outfile){
    FILE *fp;
    if(outfile != stdout) fp = fopen(outfile, "w");
    else fp = stdout;

    for(int i=0; i < tab->n_entries; i++) printEntry(tab->log[i], fp);
    if(outfile != stdout) fclose(fp);
}

void printEntry(entry_t e, FILE *fp){
    fprintf(fp, "%s %s %s %s %s %s %d\n", e.code, e.departure, e.arrival, e.date_str, e.timeD_str, e.timeA_str, e.delay);
}

void insertionSort(table_t *tab, key_e key){
    int i, j, l=0, r = tab->n_entries-1;
    entry_t e;

    for(i=l+1; i<=r; i++){
        e = tab->log[i];
        j=i-1;
        while(j>=l && compareEntries(e, tab->log[j], key) < 0){
            tab->log[j+1] = tab->log[j];
            j--;
        }
        tab->log[j+1] = e;
    }
}

int compareEntries(entry_t e1, entry_t e2, key_e k){
    int cmp;
    switch(k){
        case DATE:
            cmp = compareDates(e1.date, e2.date);
            if(cmp == 0) return compareTimes(e1.timeD, e2.timeD);
            return cmp;
        case CODE:
            return strcmp(e1.code, e2.code);
        case DEPARTURE:
            return strcmp(e1.departure, e2.departure);
        case ARRIVAL:
            return strcmp(e1.arrival, e2.arrival);
        default:
            break;
    }
    return -1;
}

int compareDates(date_t d1, date_t d2){
    if(d1.year != d2.year) return (d1.year-d2.year);
    else if(d1.month != d2.month) return (d1.month-d2.month);
    else if(d1.day != d2.day) return (d1.day-d2.day);
    else return 0;
}

int compareTimes(time_t t1, time_t t2){
    if(t1.h != t2.h) return (t1.h-t2.h);
    else if(t1.min != t2.min) return (t1.min-t2.min);
    else if(t1.sec != t2.sec) return (t1.sec-t2.sec);
    else return 0;
}

int linearSearchCode(table_t *tab, char *code){
    int i=0, found=0;
    while(i < tab->n_entries){
        if(strcmp(tab->log[i].code, code)==0){
            printEntry(tab->log[i], stdout);
            found = 1;
        }
        i++;
    }
    if(!found) printf("Entry not found!\n");
}

int binarySearchCode(table_t *tab, char *code){
    int found=0, l=0, r=tab->n_entries-1, m, cmp, i, j;

    while(l<=r && !found){
        m = (l+r)/2;
        cmp = strcmp(tab->log[m].code, code);
        if(cmp==0) found=1;
        else if(cmp<0) l = m+1;
        else r = m-1;
    }

    if(found){                                                                          // at least one entry was found
        i=m;
        j=m-1;
        while(i<tab->n_entries && strcmp(tab->log[i].code, code)==0){                   // prints matching entries on the right
            printEntry(tab->log[i], stdout);
            i++;
        }
        while(j>=0 && strcmp(tab->log[j].code, code)==0){                               // prints matching entries on the left
            printEntry(tab->log[j], stdout);
            j--;
        }
    }
    else printf("Entry not found!\n");
}

void linearSearchStation(table_t *tab, char *station, int n){
    int i=0, found=0;
    while(i < tab->n_entries){
        if(strncmp(tab->log[i].departure, station, n)==0){
            printEntry(tab->log[i], stdout);
            found = 1;
        }
        i++;
    }
    if(!found) printf("Entry not found!\n");
}

void binarySearchStation(table_t *tab, char *station, int n){
    int found=0, l=0, r=tab->n_entries-1, m, cmp, i, j;

    while(l<=r && !found){
        m = (l+r)/2;
        cmp = strncmp(tab->log[m].departure, station, n);
        if(cmp==0) found=1;
        else if(cmp<0) l = m+1;
        else r = m-1;
    }

    if(found){
        i=m;
        j=m-1;
        while(i<tab->n_entries && strncmp(tab->log[i].departure, station, n)==0){
            printEntry(tab->log[i], stdout);
            i++;
        }
        while(j>=0 && strncmp(tab->log[j].departure, station, n)==0){
            printEntry(tab->log[j], stdout);
            j--;
        }
    }
    else printf("Entry not found!\n");
}