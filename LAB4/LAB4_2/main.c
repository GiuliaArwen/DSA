#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX1 51
#define CMD 7

typedef struct{
    int d;
    int m;
    int y;
} date_t;

typedef struct{
    char *road;
    char *town;
    int zip;
} address_t;

typedef struct{
    char *code;
    char *name;
    char *surname;
    char *date_str;
    date_t date;
    address_t address;
} Item_t;

typedef struct node *link;

typedef struct node{
    Item_t *val;
    link next;
} node_t;
 
link newNode(Item_t *val, link next);
link sortedInsert(link h, Item_t *val);
link addItem(link head);
link loadFile(link head, char *filename);
Item_t *readItem(FILE *fp);
Item_t *newItem(char *code, char *name, char *surname, date_t date, char *road, char *town, int zip);
Item_t *searchCode(link head, char *code);
Item_t *deleteCode(link *head, char *code);
Item_t *deleteDates(link *head, date_t date1, date_t date2);
date_t breakdown(char *date_str);
int compareDate(date_t d1, date_t d2);
int printMenu(char *commands[]);
void swap(date_t *d1, date_t *d2);
void printItem(Item_t *item, FILE *fp);
void printFile(link head, char *filename);
void freeItem(Item_t *item);
void freeFile(link head);

int main(void){

    char *commands[] = {
        "Load from terminal",
        "Load from file",
        "Search by code",
        "Extract through code",
        "Extract between dates",
        "Print on file",
        "Exit"
    };
    char filename[MAX1], inputCode[MAX1], inputDate1[MAX1], inputDate2[MAX1];
    date_t date1, date2;
    int choice, proceed = 1, true=1;
    link head = NULL;
    Item_t *item;

    do{
        choice = printMenu(commands);
        switch(choice){
            case 0:
                head = addItem(head);
                break;
            case 1:
                printf("Enter file name: ");
                scanf("%s", filename);
                head = loadFile(head, filename);
                break;
            case 2:
                printf("Enter code to search: ");
                scanf("%s", inputCode);
                item = searchCode(head, inputCode);
                if(item != NULL) printItem(item, stdout);
                else fprintf(stdout, "Code not found!\n");
                break;
            case 3:
                printf("Enter code to delete: ");
                scanf("%s", inputCode);
                item = deleteCode(&head, inputCode);
                if(item != NULL){
                    printItem(item, stdout);
                    freeItem(item);
                }
                else fprintf(stdout, "Code not found!\n");
                break;
            case 4:
                printf("Enter first date: ");
                scanf("%s", inputDate1);
                printf("Enter second date: ");
                scanf("%s", inputDate2);
                date1 = breakdown(inputDate1);
                date2 = breakdown(inputDate2);
                if(compareDate(date1, date2) > 0) swap(&date1, &date2);
                do{
                    item = deleteDates(&head, date1, date2);    // only returns first occurrence, hence the need for a do-while loop
                    if(item!=NULL){
                        printItem(item, stdout);
                        freeItem(item);
                    }
                    else{
                        printf("\nEnd of deletion\n");
                        break;
                    }
                } while(true);                                  // flag to make the loop run indefinitely until it breaks
                break;
            case 5:
                printf("Enter output file name: ");
                scanf("%s", filename);
                printFile(head, filename);
                break;
            case 6:
                proceed=0;
                break;
            default:
                printf("Choice not supported!");
                break;
        }
    } while(proceed);

    freeFile(head);
    head = NULL;
    return 0;
}

int printMenu(char *commands[]){
    int choice;
    printf("\nMENU\n");
    for(int i=0; i<CMD; i++) printf("%d > %s\n", i, commands[i]);
    scanf("%d", &choice);
    return choice;
}

link addItem(link head){
    Item_t *item;
    printf("Code Name Surname dd/mm/yyyy Road City Zip: ");
    item = readItem(stdin);
    if(item != NULL) head = sortedInsert(head, item);           // sorted head insertion of new entry
    return head;
}

Item_t *readItem(FILE *fp){
    Item_t *item = NULL;
    int zip;
    char tmpCode[MAX1], tmpName[MAX1], tmpSurname[MAX1], tmpDate[MAX1], tmpRoad[MAX1], tmpTown[MAX1];

    if(fscanf(fp, "%s %s %s %s %s %s %d", tmpCode, tmpName, tmpSurname, tmpDate, tmpRoad, tmpTown, &zip)!=7) return NULL;
    item = newItem(tmpCode,tmpName, tmpSurname, breakdown(tmpDate), tmpRoad, tmpTown, zip);

    return item;
}

date_t breakdown(char *date_str){
    date_t date;
    sscanf(date_str, "%d/%d/%d", &date.d, &date.m, &date.y);
    return date;
}

Item_t *newItem(char *code, char *name, char *surname, date_t date, char *road, char *town, int zip){
    Item_t *item = (Item_t*)malloc(sizeof(Item_t));
    if(item == NULL) return NULL;

    item->code = strdup(code);
    item->name = strdup(name);
    item->surname = strdup(surname);
    item->date = date;
    item->address.road = strdup(road);
    item->address.town = strdup(town);
    item->address.zip = zip;

    return item;
}

link sortedInsert(link h, Item_t *val){
    link p=NULL, x=NULL;
    if(h==NULL || compareDate(h->val->date, val->date)>0) return newNode(val, h);                   // if the list is empty or if the new node's date is earlier than the head's, it becomes the new head node
    for(x=h->next, p=h; x!=NULL && compareDate(val->date, x->val->date)>0; p=x, x=x->next);         // otherwise p iterates from the head, x from the succeeding node, until the current date is lower thant the next
    p->next = newNode(val, x);                                                                      // adds between p and x a node with the new value
    return h;
}

int compareDate(date_t d1, date_t d2){
    if(d1.y != d2.y) return (d1.y-d2.y);
    else if(d1.m != d2.m) return (d1.m-d2.m);
    else if(d1.d != d2.d) return (d1.d-d2.d);
    else return 0;
}

link newNode(Item_t *val, link next){
    link x = malloc(sizeof(*x));
    if(x==NULL) return NULL;
    x->val = val;
    x->next = next;
    return x;
}

link loadFile(link head, char *filename){
    Item_t *item;

    FILE *fp = fopen(filename, "r");
    if(fp == NULL) exit (-1);

    while((item=readItem(fp))!=NULL) head = sortedInsert(head, item);

    fclose(fp);
    return head;
}

Item_t *searchCode(link head, char *code){
    link x;
    for(x = head; x != NULL; x = x->next)                       // iteration on the whole list
        if(strcmp(x->val->code, code) == 0) return x->val;
    return NULL;
}

Item_t *deleteCode(link *head, char *code){
    link x, p;
    Item_t *item = NULL;
    for(x = *head, p = NULL; x!= NULL; p=x, x = x->next){
        if(strcmp(x->val->code, code) == 0){
            if(p == NULL) *head = x->next;                      // if the node to be deleted is the head, updates the head pointer
            else p->next = x->next;                             // otherwise skips the node to be deleted and updates next pointer
            item = x->val;                                      // stores deleted value in item
            free(x);                                            // frees memory allocated for deleted node
            break;
        }
    }
    return item;                                                // extraction: returns the deleted item
}

void printItem(Item_t *item, FILE *fp){
    fprintf(fp, "%s\t%s %s\t%d/%d/%d\t%s %s %d\n", item->code, item->name, item->surname, item->date.d, item->date.m, item->date.y, item->address.road, item->address.town, item->address.zip);
}

void freeItem(Item_t *item){
    if(item == NULL) return;
    if(item->code != NULL) free(item->code);
    if(item->name != NULL) free(item->name);
    if(item->surname != NULL) free(item->surname);
    if(item->address.road != NULL) free(item->address.road);
    if(item->address.town != NULL) free(item->address.town);
    free(item);
}

void swap(date_t *date1, date_t *date2){
    date_t tmp;
    tmp = *date1;
    *date1 = *date2;
    *date2 = tmp;
}

Item_t *deleteDates(link *head, date_t date1, date_t date2){
    link x, p;
    Item_t *item = NULL;
    for(x = *head, p = NULL; x != NULL && compareDate(date2, x->val->date)>=0; p=x, x=x->next){     // iterates until the second input date is greater than the current one
        if(compareDate(date1, x->val->date)<=0){                                                    // checks wheter the current date belongs to the input date range
            if(p==NULL) *head = x->next;
            else p->next = x->next;
            item = x->val;
            free(x);
            break;
        }
    }
    return item;
}

void printFile(link head, char *filename){
    link x = NULL;
    FILE *fp = fopen(filename, "w");
    for(x=head; x!=NULL; x=x->next) printItem(x->val, fp);
    if(fp!=NULL) fclose(fp);
}

void freeFile(link head){
    link x, t;
    for(x = head; x != NULL; x = t){                            // t variable is needed as the next reference will be lost while freeing the current node!
        t = x->next;
        freeItem(x->val);
        free(x);
    }
}