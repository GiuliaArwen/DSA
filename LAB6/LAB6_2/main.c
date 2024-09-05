#include <stdio.h>
#include <stdlib.h>
#define FILENAME "hard_test_set.txt"
#define N 4

void solve(int *gemsQTY);
int ****alloc4d(int *gemsQTY);
int fS(int ****S, int ****R, int ****T, int ****E, int s, int r, int t, int e);
int fR(int ****S, int ****R, int ****T, int ****E, int s, int r, int t, int e);
int fT(int ****S, int ****R, int ****T, int ****E, int s, int r, int t, int e);
int fE(int ****S, int ****R, int ****T, int ****E, int s, int r, int t, int e);
int max(int a, int b);

int main(void){
    int gemsQTY[N], maxL, NTest;
    FILE *fp = fopen(FILENAME, "r");
    if(fp == NULL) return -1;

    fscanf(fp, "%d", &NTest);
    for(int i=0; i<NTest; i++){
        printf("TEST #%d\n", i+1);
        maxL=0;
        for(int j=0; j<N; j++){
            fscanf(fp, "%d", &gemsQTY[j]);
            maxL += gemsQTY[j];
        }
        printf("Maximum length: %d\n", maxL);
        solve(gemsQTY);
    }
    fclose(fp);
    return 0;
}

void solve(int *gemsQTY){
    int maxS, maxR, maxT, maxE;
    int ****S, ****R, ****T, ****E;
    int res, s=gemsQTY[0], r=gemsQTY[1], t=gemsQTY[2], e=gemsQTY[3];
    printf("Sapphires = %d\tRubies = %d\tTopazes = %d\tEmeralds = %d\n", s,r,t,e);

    S = alloc4d(gemsQTY);                               // memoization: each matrix stores the values of the longest necklace starting with its gem
    R = alloc4d(gemsQTY);
    T = alloc4d(gemsQTY);
    E = alloc4d(gemsQTY);

    maxS = fS(S, R, T, E, s, r, t, e);                  // each function returns the maximum length for each necklace starting with its gem
    maxR = fR(S, R, T, E, s, r, t, e);
    maxT = fT(S, R, T, E, s, r, t, e);
    maxE = fE(S, R, T, E, s, r, t, e);

    res = max(maxS, max(maxR, max(maxT, maxE)));        // finally chooses the longest out of them all
    printf("Necklace with maximum length %d\n", res);
}

int ****alloc4d(int *gemsQTY){
    int ****mat = malloc((1+gemsQTY[0])*sizeof(int***));
    for(int i=0; i<=gemsQTY[0]; i++){
        mat[i] = malloc((1+gemsQTY[1])*sizeof(int**));
        for(int j=0; j<=gemsQTY[1]; j++){
            mat[i][j] = malloc((1+gemsQTY[2])*sizeof(int*));
            for(int k=0; k<=gemsQTY[2]; k++){
                mat[i][j][k] = malloc((1+gemsQTY[3])*sizeof(int));
                for(int l=0; l<=gemsQTY[3]; l++)
                    mat[i][j][k][l] = -1;               // initializes at -1
            }
        }
    }
    return mat;
}

int fS(int ****S, int ****R, int ****T, int ****E, int s, int r, int t, int e){
    int nextS, nextR;                                   // given problem criteria
    if(s <= 0) return 0;                                // exit condition: run out of stones
    if(S[s][r][t][e] != -1) return S[s][r][t][e];       // memoization: return if matrix already filled!
    nextS = fS(S,R,T,E,s-1,r,t,e);                      // decrease by 1 the current gem
    nextR = fR(S,R,T,E,s-1,r,t,e);
    S[s][r][t][e] = 1+max(nextS, nextR);                // adds current stone to max
    return S[s][r][t][e];
}

int fR(int ****S, int ****R, int ****T, int ****E, int s, int r, int t, int e){
    int nextE, nextT;
    if(r <= 0) return 0;
    if(R[s][r][t][e] != -1) return R[s][r][t][e];
    nextE = fE(S,R,T,E,s,r-1,t,e);
    nextT = fT(S,R,T,E,s,r-1,t,e);
    R[s][r][t][e] = 1+max(nextE, nextT);
    return R[s][r][t][e];
}

int fT(int ****S, int ****R, int ****T, int ****E, int s, int r, int t, int e){
    int nextS, nextR;
    if(t <= 0) return 0;
    if(T[s][r][t][e] != -1) return T[s][r][t][e];
    nextS = fS(S,R,T,E,s,r,t-1,e);
    nextR = fR(S,R,T,E,s,r,t-1,e);
    T[s][r][t][e] = 1+max(nextS,nextR);
    return T[s][r][t][e];
}

int fE(int ****S, int ****R, int ****T, int ****E, int s, int r, int t, int e){
    int nextE, nextT;
    if(e <= 0) return 0;
    if(E[s][r][t][e] != -1) return E[s][r][t][e];
    nextE = fE(S,R,T,E,s,r,t,e-1);
    nextT = fT(S,R,T,E,s,r,t,e-1);
    E[s][r][t][e] = 1+max(nextE, nextT);
    return E[s][r][t][e];
}

int max(int a, int b){
    if(a>b) return a;
    return b;
}