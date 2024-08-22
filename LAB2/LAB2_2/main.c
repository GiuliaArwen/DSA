#include <stdio.h>
#include <stdlib.h>
#define FILE_NAME "mat.txt"

int **malloc2DR(FILE *fp, int *nr, int *nc);
void malloc2DP(FILE *fp, int ***matrix, int *nr, int *nc);
void divide(int **matrix, int nr, int nc, int **white, int **black, int *nw, int *nb);
void printOut(int *vect, int n);

int main(void){
    int **matrix, nr, nc;
    int *white, *black, nw, nb;
    FILE *fp = fopen(FILE_NAME, "r");
    if(fp == NULL) return -1;

    //matrix = malloc2DR(fp, &nr, &nc);
    malloc2DP(fp, &matrix, &nr, &nc);

    divide(matrix, nr, nc, &white, &black, &nw, &nb);
    printf("White cells\n");
    printOut(white, nw);
    printf("Black cells\n");
    printOut(black, nb);

    free(white);
    free(black);
    for(int i=0; i<nr; i++) free(matrix[i]);
    free(matrix);
    fclose(fp);
    return 0;
}

int **malloc2DR(FILE *fp, int *nr, int *nc){
    int **mat;
    fscanf(fp, "%d %d", nr, nc);
    mat = (int **) calloc(*nr, sizeof(int *));
    for(int i=0; i < *nr; i++){
        mat[i] = (int *) calloc(*nc, sizeof(int));
        for(int j=0; j < *nc; j++){
            fscanf(fp, "%d", &mat[i][j]);
        }
    }
    return mat;
}

void malloc2DP(FILE *fp, int ***matrix, int *nr, int *nc){
    int **mat;
    fscanf(fp, "%d %d", nr, nc);
    mat = (int **) calloc(*nr, sizeof(int *));
    for(int i=0; i < *nr; i++){
        mat[i] = (int *) calloc(*nc, sizeof(int));
        for(int j=0; j < *nc; j++){
            fscanf(fp, "%d", &mat[i][j]);
        }
    }
    *matrix = mat;
}

void divide(int **matrix, int nr, int nc, int **white, int **black, int *nw, int *nb){
    int cntw=0, cntb=0, *w, *b;
    *nw = (nr*nc)/2;
    *nb = (nr*nc)/2 + (nr*nc)%2;
    w = (int *) calloc(*nw, sizeof(int));
    b = (int *) calloc(*nb, sizeof(int));
    for(int i=0; i<nr; i++){
        for(int j=0; j<nc; j++){
            if((i+j)%2) w[cntw++] = matrix[i][j];
            else b[cntb++] = matrix[i][j];
        }
    }
    *white = w;
    *black = b;
}

void printOut(int *vect, int n){
    for(int i=0; i<n; i++) printf("%d ", vect[i]);
    printf("\n");
}