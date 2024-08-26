#include <stdio.h>
#include <stdlib.h>
#define MAX 4
#define FILENAME "easy_test_set.txt"

int rep_disp(int pos, int *gemsDISP, int *sol, int n, int k, int prev);

int main(void){
    int N, gemsQTY[MAX], gemsDISP[MAX], gemsUSED[MAX], min, max, bestL, *sol;
    char gems[MAX] = {'S', 'R', 'T', 'E'}, *gemsNAMES[MAX] = {"Sapphires", "Rubies", "Topazes", "Emeralds"};

    FILE *fp = fopen(FILENAME, "r");
    if (fp == NULL) return -1;

    fscanf(fp, "%d", &N);                                       // gets the number of sets to consider
    for(int i=0; i<N; i++){
        for (int j = 0; j < MAX; j++) gemsUSED[j] = 0;
        printf("\nTEST #%d\n", i+1);
        min=0;
        max=0;
        for(int j=0; j<MAX; j++){
            fscanf(fp, "%d", &gemsQTY[j]);                      // gets the set's quantity for each stone
            max += gemsQTY[j];                                  // the maximum length is the one achieved by using all the stones available
        }
        sol = malloc((max)*sizeof(int));                        // the solution has the size of max at most

        while(min <= max){                                      // binary seach to find the best length
            int k = min + (max-min)/2;
            for(int j=0; j<MAX; j++) gemsDISP[j] = gemsQTY[j];  // creates a copy of the original quantity to be modified in the recursion
            if(rep_disp(0, gemsDISP, sol, MAX, k, -1)){         // if a solution is found for k:
                bestL = k;                                      // updates bestL
                min = k+1;                                      // keeps looking for greater lengths
            }
            else max = k-1;                                     // otherwise looks for minor lengths
        }
        printf("Best length (%d) match is:\n", bestL);
        for(int j=0; j<bestL; j++){
            printf("%c", gems[sol[j]]);                         // associates each index to the corresponding stone
            gemsUSED[sol[j]]++;
        }
        printf("\n");
        for(int j=0; j<MAX; j++) printf("%s: %d\t", gemsNAMES[j], gemsUSED[j]);
        free(sol);
    }
    fclose(fp);
    return 0;
}

int rep_disp(int pos, int *gemsDISP, int *sol, int n, int k, int prev){                 // recursive function to find solutions of given length k
    if(pos >= k) return 1;                                                              // exit condition: the solution is completed
    for(int i=0; i<n; i++){                                                             // loops through each gem type
        if(gemsDISP[i]>0){                                                              // as long as there are gems available for said type
            if(prev != -1){                                                             // compatibility check with the previous gem
                if(((sol[prev]==0) || (sol[prev]==2)) && (i!=0) && (i!=1)) continue;
                if(((sol[prev]==3) || (sol[prev]==1)) && (i!=2) && (i!=3)) continue;
            }
            gemsDISP[i]--;
            sol[pos] = i;
            if(rep_disp(pos+1, gemsDISP, sol, n, k, pos)) return 1;
            gemsDISP[i]++;                                                               // backtrack in case of failure
        }
    }
    return 0;
}