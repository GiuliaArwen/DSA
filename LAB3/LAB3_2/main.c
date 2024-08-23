#include <stdio.h>
#include <stdlib.h>
#define FILENAME "songs.txt"
#define MAX 256

typedef struct{
    int nS;         // number of songs
    char **title;   // vector of song titles
} friend_t;

friend_t *readFile(char *filename, int *nF);
void playlist(friend_t *friend, int nF);
void mult_princ(int pos, friend_t *friend, int *sol, int nF, int *cnt);
void freeFriend(friend_t *friend, int nF);

int main(void){
    int nF;         // number of friends
    friend_t *friend = readFile(FILENAME, &nF);
    playlist(friend, nF);
    freeFriend(friend, nF);
    return 0;
}

friend_t *readFile(char *filename, int *nF){
    friend_t *friend;
    char tmpTitle[MAX];

    FILE *fp = fopen(filename, "r");
    if(fp == NULL) exit (-1);

    fscanf(fp, "%d", nF);
    friend = calloc(*nF, sizeof(friend_t));
    if(friend == NULL) exit(-1);

    for(int i=0; i<*nF; i++){
        fscanf(fp, "%d", &friend[i].nS);
        friend[i].title = calloc(friend[i].nS, sizeof(char*));
        for(int j=0; j<friend[i].nS; j++){
            fscanf(fp, "%s", tmpTitle);
            friend[i].title[j] = strdup(tmpTitle);
        }
    }
    return friend;
}

void playlist(friend_t *friend, int nF){        // wrapper for the multiplication principle function
    int *sol, cnt=0;                            // counter for solutions discovered
    sol = calloc(nF, sizeof(friend_t));         // the dimension of the playlist is the same as the number of friends
    mult_princ(0, friend, sol, nF, &cnt);
    free(sol);
}

void mult_princ(int pos, friend_t *friend, int *sol, int nF, int *cnt){
    if(pos == nF){      // exiting condition
        printf("Playlist #%d\n", ++(*cnt));
        for(int i=0; i<pos; i++) printf(" * %s\n", friend[i].title[sol[i]]);
        printf("\n");
        return;
    }
    for(int i=0; i<friend[pos].nS; i++){
        sol[pos] = i;
        mult_princ(pos+1, friend, sol, nF, cnt);
    }
}

void freeFriend(friend_t *friend, int nF){
    for(int i=0; i<nF; i++) free(friend[i].title);
    free(friend);
}