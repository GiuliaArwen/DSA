#include <stdio.h>
#include <stdlib.h>

void majorityWrap(int *vect, int n);
int majority(int *vect, int n, int l, int r);
int count(int *vect, int n, int l, int r, int res);

int main(int argc, char **argv){
    if(argc<2) return -1;

    int n = atoi(argv[1]);
    int *vect = (int*) malloc(n*sizeof(int));

    printf("Enter vector elements: ");
    for(int i=0; i<n; i++) scanf("%d", &vect[i]);

    majorityWrap(vect, n);

    return 0;
}

void majorityWrap(int *vect, int n){
    int l=0, r=n-1, res;
    res = majority(vect, n, l, r);
    if(res == -1) printf("No majority element found!\n");
    else printf("Majority element obtained through O(nlogn) algorithm: %d\n", res);
}

int majority(int *vect, int n, int l, int r){               // divide the vect in two to search for the majority element
    int resL, resR, resL_cnt, resR_cnt;
    int m = (l+r)/2;
    if(l==r) return vect[l];                                // vector made of one element

    resL = majority(vect, n, l, m);                         // recursive search on the left half
    resR = majority(vect, n, m+1, r);                       // recursive search on the right half

    if(resL == resR) return resL;                           // same element for both sides

    resL_cnt = count(vect, n, l, r, resL);                  // counter for resL occurences
    resR_cnt = count(vect, n, l, r, resR);                  // counter for resR occurences

    if(resL_cnt > (n-l)/2) return resL;                     // count needs to exceed half of the original length
    else if(resR_cnt > (n-l)/2) return resR;
    else return -1;
}

int count(int *vect, int n, int l, int r, int res){
    int cnt=0;
    for(int i=0; i<n; i++)
        if(vect[i] == res) cnt++;
    return cnt;
}