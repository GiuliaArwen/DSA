#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "item.h"
#include "ST.h"
#include "graph.h"

#define FILENAME "graph.txt"
#define LEN 31

Graph readFile(FILE *fp, ST st);

int main(void){
    char node[LEN], select[LEN], *sel;
    Graph g = NULL;
    ST st = STinit(1);
    FILE *fp = fopen(FILENAME, "r");
    if(fp == NULL) exit(-1);
    int id1, id2, id3, stop=0, listgen=0;
    g = readFile(fp, st);
    if(g == NULL || st == NULL) exit(-1);

    do{
        printf("\nWhich action do you want to take?\n");
        printf("\tA: View graph\n");
        printf("\tB: Generate adjacent list\n");
        printf("\tC: Control 3-clique\n");
        printf("\tZ: Exit\n");
        scanf("%s", select);
        for(sel = select; !isalpha(*sel); sel++);
        *sel = toupper(*sel);
        switch(*sel){
            case 'A':
                GRAPHstore(g, st);
                break;

            case 'B':
                if(!GRAPHmat2list(g))
                    printf("Succesful generation!\n");
                listgen = 1;
                break;
            case 'C':
                printf("Enter first node's name: ");
                KEYscan(node);
                id1 = STsearch(st, node);
                printf("Enter second node's name: ");
                KEYscan(node);
                id2 = STsearch(st, node);
                printf("Enter third node's name: ");
                KEYscan(node);
                id3 = STsearch(st, node);
                if((id1 != -1) && (id2 != -1) && (id3 != -1)) printf("Subgraph %s complete.\n", GRAPHcheck3clique(g, id1, id2, id3) ? "is" : "isn't");
                break;
            case 'Z':
                stop = 1;
                break;
            default:
                printf("Invalid option.\n");
        }
    } while(!stop);

    STfree(st);
    GRAPHfree(g, listgen);
    return 0;
}

Graph readFile(FILE *fp, ST st){
    int i, id1, id2;
    Item x;
    char c1[LEN], n1[LEN], c2[LEN], n2[LEN];
    Graph g;

    while(fscanf(fp, "%s %s %s %s %d\n", c1, n1, c2, n2, &i) == 5){
        x = ITEMload(c1, n1);
        id1 = STsearch(st, KEYget(&x));
        if(id1 == -1) STinsert(st, x);
        x = ITEMload(c2, n2);
        id2 = STsearch(st, KEYget(&x));
        if(id2 == -1) STinsert(st, x);
    }

    rewind(fp);

    g = GRAPHinit(STcount(st));
    if(g == NULL) return NULL;

    while(fscanf(fp, "%s %s %s %s %d", c1, n1, c2, n2, &i) == 5){
        x = ITEMload(c1, n1);
        id1 = STsearch(st, KEYget(&x));
        x = ITEMload(c2, n2);
        id2 = STsearch(st, KEYget(&x));
        if(id1 != id2 && id1 >= 0 && id2 >= 0) GRAPHinsertE(g, id1, id2, i);
    }
    
    return g;
}