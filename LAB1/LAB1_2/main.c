#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ROW 1000
#define MAX_STR 31
#define CMD 6
#define FILE_NAME "../log.txt"

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
} table_t;

typedef enum {r_date, r_departure, r_arrival, r_delay, r_TOTdelay, r_end} command_e;            // standard procedure to handle menu interface

table_t readTable();
void printCommands(char commands[][MAX_STR]);
command_e readCommand(char commands[][MAX_STR]);
void selectData(table_t tab, command_e cmd);
int compareDates(date_t d1, date_t d2);
void printEntry(entry_t e);

int main(void){
    char commands[][MAX_STR] = {"date", "departure", "arrival", "delay", "TOTdelay", "end"};
    int proceed = 1;
    command_e cmd;
    table_t tab;

    tab = readTable();
    printCommands(commands);

    do{
        cmd = readCommand(commands);
        switch(cmd){
        case r_date:
            selectData(tab, r_date);
            break;
        case r_departure:
            selectData(tab, r_departure);
            break;
        case r_arrival:
            selectData(tab, r_arrival);
            break;
        case r_delay:
            selectData(tab, r_delay);
            break;
        case r_TOTdelay:
            selectData(tab, r_TOTdelay);
            break;
        case r_end:
            proceed = 0;
            break;
        default:
            printCommands(commands);
        }
    } while(proceed);
    return 0;
}

table_t readTable(){
    FILE *fp = fopen(FILE_NAME, "r");
    if (fp == NULL) exit (-1);
    table_t tab;
    
    fscanf(fp, "%d", &tab.n_entries);
    for(int i=0; i<tab.n_entries; i++){
        fscanf(fp, "%s %s %s %s %s %s %d", tab.log[i].code, tab.log[i].departure, tab.log[i].arrival, tab.log[i].date_str, tab.log[i].timeD_str, tab.log[i].timeA_str, &tab.log[i].delay);
        sscanf(tab.log[i].date_str, "%d/%d/%d", &tab.log[i].date.year, &tab.log[i].date.month, &tab.log[i].date.day);
        sscanf(tab.log[i].timeD_str, "%d:%d:%d", &tab.log[i].timeD.h, &tab.log[i].timeD.min, &tab.log[i].timeD.sec);
        sscanf(tab.log[i].timeA_str, "%d:%d:%d", &tab.log[i].timeA.h, &tab.log[i].timeA.min, &tab.log[i].timeA.sec);
    }
    fclose(fp);
    return tab;
}

void printCommands(char commands[][MAX_STR]){
    printf("Available commands:\t\t");
    for(int i=0; i<CMD; i++) printf("%s\t", commands[i]);
    printf("\n");
}

command_e readCommand(char commands[][MAX_STR]){
    char cmd[MAX_STR];
    printf("\nCommand > ");
    scanf("%s", cmd);
    for(int i=0; i<CMD; i++){
        if(strcmp(cmd, commands[i])==0) return (command_e) i;                                   // returns the corresponding enumerated command
    }
    return -1;
}

void selectData(table_t tab, command_e cmd){                                                    // switch case to handle user inputs according to each command
    date_t d1, d2;
    char input[MAX_STR];
    int tot = 0;
    if(cmd == r_date){
        printf("Enter first date (YYYY/MM/DD): ");
        scanf("%d/%d/%d", &d1.year, &d1.month, &d1.day);
        printf("Enter second date (YYYY/MM/DD): ");
        scanf("%d/%d/%d", &d2.year, &d2.month, &d2.day);
    }
    else if(cmd == r_departure){
        printf("Enter departure station: ");
        scanf("%s", input);
    }
    else if(cmd == r_arrival){
        printf("Enter arrival station: ");
        scanf("%s", input);
    }
    else if(cmd == r_delay){
        printf("Enter first date (YYYY/MM/DD): ");
        scanf("%d/%d/%d", &d1.year, &d1.month, &d1.day);
        printf("Enter second date (YYYY/MM/DD): ");
        scanf("%d/%d/%d", &d2.year, &d2.month, &d2.day);
    }
    else if(cmd == r_TOTdelay){
        printf("Enter route code: ");
        scanf("%s", input);
    }

    for(int i=0; i<tab.n_entries; i++){                                                         // for loop to check matches with the table
        switch(cmd){
            case r_date:
                if(compareDates(d1, tab.log[i].date)<=0 && compareDates(tab.log[i].date, d2)<=0)
                    printEntry(tab.log[i]);
                break;
            case r_departure:
                if(!strcmp(tab.log[i].departure, input))                                        // NOTE: !strcmp is equivalent to strcmp(s1,s2) == 0
                    printEntry(tab.log[i]);
                break;
            case r_arrival:
                if(!strcmp(tab.log[i].arrival, input))
                    printEntry(tab.log[i]);
                break;
            case r_delay:
                if(compareDates(d1, tab.log[i].date)<=0 && compareDates(tab.log[i].date, d2)<=0 && tab.log[i].delay >= 0)
                    printEntry(tab.log[i]);
                break;
            case r_TOTdelay:
                if(!strcmp(tab.log[i].code, input))
                    tot += tab.log[i].delay;
                break;
        }
    }
    if(cmd == r_TOTdelay) printf("Cumulative delay of %d min\n", tot);
}

int compareDates(date_t d1, date_t d2){                                                          // function needed to evaluate dates falling within a range
    if(d1.year != d2.year) return (d1.year-d2.year);
    else if(d1.month != d2.month) return (d1.month-d2.month);
    else if(d1.day != d2.day) return (d1.day-d2.day);
    else return 0;
}

void printEntry(entry_t e){
    printf("Route %s from %s to %s on %s (from %s to %s, delayed by %d minute%c\n", e.code, e.departure, e.arrival, e.date_str, e.timeD_str, e.timeA_str, e.delay, ((e.delay == 1)?' ':'s'));
}