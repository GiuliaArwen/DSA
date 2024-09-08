#ifndef ITEM_H_DEFINED
#define ITEM_H_DEFINED

#define LEN 31

typedef struct{
    char computer[LEN];
    char network[LEN];
} Item;

typedef char *Key;

Item    ITEMload(char name1[], char name2[]);
void    ITEMstore(Item val);
Item    ITEMsetNull();
Key     KEYget(Item *val);
void    KEYscan(char node[]);
int     KEYcmp(Key k1, Key k2);

#endif