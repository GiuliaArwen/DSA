#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STR 31
#define CMD 10
#define FILE_NAME "log.txt"

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
    char *code;
    char *departure;
    char *arrival;
    char *date_str;
    char *timeD_str;
    char *timeA_str;
    int delay;
    date_t date;
    time_t timeD, timeA;
} entry_t;

typedef struct{
    int n_entries;
    entry_t *log;
    entry_t **logDate, **logCode, **logDep, **logArr;
    key_e key;
} table_t;

void readTable(char *filename, table_t *table);
void printCommands(char *commands[], int *choice);
void printOut(table_t *tab, char *outfile, key_e key);
void insertionSort(entry_t **logX, int n_entries, key_e key);
int compareEntries(entry_t *e1, entry_t *e2, key_e k);
int compareDates(date_t d1, date_t d2);
int compareTimes(time_t t1, time_t t2);
void printEntry(entry_t e, FILE *fp);
int linearSearchCode(table_t *tab, char *code);
int binarySearchCode(table_t *tab, char *code);
void linearSearchStation(table_t *tab, char *station, int n);
void binarySearchStation(table_t *tab, char *station, int n);
void freeTable(table_t *tab);

int main(void){
    char *commands[] = {"Exit",
                        "Print on video", 
                        "Print on file", 
                        "Order by date", 
                        "Order by code", 
                        "Order by departure station",
                        "Order by arrival station",
                        "Search by code",
                        "Search by departure station",
                        "Read from file"};
    int proceed = 1, choice, nChar;
    char outfile[MAX_STR], outfile2[MAX_STR], inputCode[MAX_STR], inputStation[MAX_STR];

    table_t tab;
    readTable(FILE_NAME, &tab);

    do{
        printCommands(commands, &choice);
        switch(choice){
        case 0:
            proceed = 0;
            break;
        case 1:
            printOut(&tab, stdout, CODE);
            break;
        case 2:
            printf("Enter output file name: ");
            scanf("%s", outfile);
            printOut(&tab, outfile, CODE);
            break;
        case 3:
            printf("Ordering by date...\n");
            printOut(&tab, stdout, DATE);
            break;
        case 4:
            printf("Ordering by code...\n");
            printOut(&tab, stdout, CODE);
            break;
        case 5:
            printf("Ordering by departure station...\n");
            printOut(&tab, stdout, DEPARTURE);
            break;
        case 6:
            printf("Ordering by arrival station...\n");
            printOut(&tab, stdout, ARRIVAL);
            break;
        case 7:
            printf("Enter route code to search: ");
            scanf("%s", inputCode);
            if(tab.key == CODE) binarySearchCode(&tab, inputCode);
            else linearSearchCode(&tab, inputCode);
            break;
        case 8:
            printf("Enter departure station to search: ");
            scanf("%s%n", inputStation, &nChar);
            if(tab.key == DEPARTURE) binarySearchStation(&tab, inputStation, nChar-1);
            else linearSearchStation(&tab, inputStation, nChar-1);
            break;
        case 9:
            printf("Enter new file name: ");
            scanf("%s", outfile2);
            freeTable(&tab);
            readTable(outfile2, &tab);
            break;
        default:
            printf("Unavailable choice!");
            break;
        }
    } while(proceed);
    
    freeTable(&tab);
    return 0;
}

void readTable(char *filename, table_t *tab){
    char tmpCode[MAX_STR], tmpDep[MAX_STR], tmpArr[MAX_STR], tmpDate[MAX_STR], tmpTimeD[MAX_STR], tmpTimeA[MAX_STR];
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) exit (-1);
    
    fscanf(fp, "%d", &tab->n_entries);
    tab->log = calloc(tab->n_entries, sizeof(entry_t));
    tab->logDate = calloc(tab->n_entries, sizeof(entry_t *));
    tab->logCode = calloc(tab->n_entries, sizeof(entry_t *));
    tab->logDep = calloc(tab->n_entries, sizeof(entry_t *));
    tab->logArr = calloc(tab->n_entries, sizeof(entry_t *));

    for(int i=0; i<tab->n_entries; i++){
        fscanf(fp, "%s %s %s %s %s %s %d", tmpCode, tmpDep, tmpArr, tmpDate, tmpTimeD, tmpTimeA, &tab->log[i].delay);
        tab->log[i].code = strdup(tmpCode);
        tab->log[i].departure = strdup(tmpDep);
        tab->log[i].arrival = strdup(tmpArr);
        tab->log[i].date_str = strdup(tmpDate);
        tab->log[i].timeD_str = strdup(tmpTimeD);
        tab->log[i].timeA_str = strdup(tmpTimeA);
        sscanf(tab->log[i].date_str, "%d/%d/%d", &tab->log[i].date.year, &tab->log[i].date.month, &tab->log[i].date.day);
        sscanf(tab->log[i].timeD_str, "%d:%d:%d", &tab->log[i].timeD.h, &tab->log[i].timeD.min, &tab->log[i].timeD.sec);
        sscanf(tab->log[i].timeA_str, "%d:%d:%d", &tab->log[i].timeA.h, &tab->log[i].timeA.min, &tab->log[i].timeA.sec);
    }

    for(int i=0; i<tab->n_entries; i++){
        tab->logDate[i] = &tab->log[i];
        tab->logCode[i] = &tab->log[i];
        tab->logDep[i] = &tab->log[i];
        tab->logArr[i] = &tab->log[i];
    }

    insertionSort(tab->logDate, tab->n_entries, DATE);
    insertionSort(tab->logCode, tab->n_entries, CODE);
    insertionSort(tab->logDep, tab->n_entries, DEPARTURE);
    insertionSort(tab->logArr, tab->n_entries, ARRIVAL);

    fclose(fp);
}

void printCommands(char *commands[], int *choice){
    printf("\nMENU:\n");
    for(int i=0; i<CMD; i++) printf("%2d\t>\t%s\n", i, commands[i]);
    scanf(" %d", choice);
}

void printOut(table_t *tab, char *outfile, key_e key){
    FILE *fp;
    if(outfile != stdout) fp = fopen(outfile, "w");
    else fp = stdout;

    switch(key){
    case DATE:
        for(int i=0; i < tab->n_entries; i++) printEntry(*(tab->logDate[i]), fp);
        break;
    case CODE:
        for(int i=0; i < tab->n_entries; i++) printEntry(*(tab->logCode[i]), fp);
        break;
    case DEPARTURE:
        for(int i=0; i < tab->n_entries; i++) printEntry(*(tab->logDep[i]), fp);
        break;
    case ARRIVAL:
        for(int i=0; i < tab->n_entries; i++) printEntry(*(tab->logArr[i]), fp);
        break;
    default:
        for(int i=0; i < tab->n_entries; i++) printEntry(*(tab->logCode[i]), fp);
        break;
    }
    
    if(outfile != stdout) fclose(fp);
}

void printEntry(entry_t e, FILE *fp){
    fprintf(fp, "%s %s %s %s %s %s %d\n", e.code, e.departure, e.arrival, e.date_str, e.timeD_str, e.timeA_str, e.delay);
}

void insertionSort(entry_t **logX, int n_entries, key_e key){                           // made changes to fit pointers to struct passed by reference
    int i, j, l=0, r = n_entries-1;
    entry_t *e;

    for(i=l+1; i<=r; i++){
        e = logX[i];
        j=i-1;
        while(j>=l && compareEntries(e, logX[j], key) < 0){
            logX[j+1] = logX[j];
            j--;
        }
        logX[j+1] = e;
    }
}

int compareEntries(entry_t *e1, entry_t *e2, key_e k){
    int cmp;
    switch(k){
        case DATE:
            cmp = compareDates(e1->date, e2->date);
            if(cmp == 0) return compareTimes(e1->timeD, e2->timeD);
            return cmp;
        case CODE:
            return strcmp(e1->code, e2->code);
        case DEPARTURE:
            return strcmp(e1->departure, e2->departure);
        case ARRIVAL:
            return strcmp(e1->arrival, e2->arrival);
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

void freeTable(table_t *tab){
    if(tab==NULL) return;
    // ordered arrays are pointers to the same memory location as log
    if(tab->logDate) free(tab->logDate);
    if(tab->logCode) free(tab->logCode);
    if(tab->logDep) free(tab->logDep);
    if(tab->logArr) free(tab->logArr);
    if(tab->log){
        for(int i=0; i < tab->n_entries; i++){
            if(tab->log[i].code) free(tab->log[i].code);
            if(tab->log[i].departure) free(tab->log[i].departure);
            if(tab->log[i].arrival) free(tab->log[i].arrival);
            if(tab->log[i].date_str) free(tab->log[i].date_str);
            if(tab->log[i].timeA_str) free(tab->log[i].timeA_str);
            if(tab->log[i].timeD_str) free(tab->log[i].timeD_str);
        }
        free(tab->log);
    }
    free(tab);
}