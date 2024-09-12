#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "quotations.h"

typedef struct node_s *link;
typedef struct node_s{
    link l, r;
    Quotation q;
    int cnt;
} Node;

struct BSTquotations_s{
    link root;
    int count;
};

static Quotation searchR(link h, Date d);
static link insertR(link h, Date d, float val, int n, int *add);
static link NEW(Date d, float val, int n, link left, link right);
static void treeRange(link root, Date d1, Date d2, float *f1, float *f2);
static void treePrint(FILE *fp, link h);
static int treeHeight(link h, int *max, int depth);
static void treeBalance(BSTquotations bst);
static link treePartition(link h, int k);
link rotR(link h);
link rotL(link h);
static void minmax(link h, float *min, float *max);

int BSTquotationsEmpty(BSTquotations bst){
    if(bst == NULL || bst->root == 0) return 1;
    return 0;
}

Quotation BSTquotationsSearch(BSTquotations bst, Date d){
    return searchR(bst->root, d);
}

static Quotation searchR(link h, Date d){
    int cmp;
    if(h == NULL) return QUOTATIONsetNull();
    cmp = DATEcmp(d, QUOTATIONgetDate(h->q));
    if(cmp == 0) return h->q;
    else if(cmp < 0) return searchR(h->l, d);
    else return searchR(h->r, d);
}

void BSTquotationsInsert(BSTquotations bst, Date d, float val, int n){
    int add = 0;
    bst->root = insertR(bst->root, d, val, n, &add);
    if(add) bst->count++;
    return;
}

static link insertR(link h, Date d, float val, int n, int *add){
    int cmp;
    link new;
    if(h == NULL){
        *add = 1;
        new = NEW(d, val, n, NULL, NULL);
        return new;
    }
    cmp = DATEcmp(d, QUOTATIONgetDate(h->q));
    if(cmp < 0)
        h->l = insertR(h->l, d, val, n, add);
    else if(cmp > 0)
        h->r = insertR(h->r, d, val, n, add);
    else{
        h->q.val += n * val;
        h->q.n += n;
    }
    if(*add) h->cnt++;
    return h;
}

static link NEW(Date d, float val, int n, link left, link right){
    link x = calloc(1, sizeof(*x));
    x->q.date = d;
    x->q.val = n * val;
    x->q.n = n;
    x->l = left;
    x->r = right;
    x->cnt = 1;
    return x;
}

void BSTquotationsRange(BSTquotations bst, Date d1, Date d2, float *f1, float *f2){
    if(bst == NULL || bst->root == NULL || bst->count == 0) return;
    treeRange(bst->root, d1, d2, f1, f2);
}

static void treeRange(link root, Date d1, Date d2, float *f1, float *f2){
    if(root == NULL) return;
    int cmp1 = DATEcmp(d1, QUOTATIONgetDate(root->q));
    int cmp2 = DATEcmp(d2, QUOTATIONgetDate(root->q));
    float qv;
    if(root == NULL) return;
    if(cmp1<0) treeRange(root->l, d1, d2, f1, f2);
    if(cmp2>0) treeRange(root->r, d1, d2, f1, f2);
    if(cmp1<=0 && cmp2>=0){
        qv = QUOTATIONgetVal(root->q);
        if(*f1 < 0 || *f1 > qv) *f1 = qv;
        if(*f2 < 0 || *f2 < qv) *f2 = qv;
    }
}

BSTquotations BSTquotationsInit(){
    BSTquotations bst = calloc(1, sizeof(*bst));
    return bst;
}

void BSTquotationsStore(FILE *fp, BSTquotations bst){
    if(BSTquotationsEmpty(bst)) return;
    treePrint(fp, bst->root);
}

static void treePrint(FILE *fp, link h){
    if(h == NULL) return;
    treePrint(fp, h->l);
    QUOTATIONstore(fp, h->q);           // ensures ascending order
    treePrint(fp, h->r);
}

void BSTquotationsBalance(BSTquotations bst){
    int min = -1, max = -1;
    if(BSTquotationsEmpty(bst)) return;
    min = treeHeight(bst->root, &max, 1);
    if(bst->root && (bst->root->l == NULL || bst->root->r == NULL))
        min = 0;
    if(min == 0 || (max / (float) min) > S){
        printf("Pre-balancing >\t\t Minimum path = %d\t Maximum path = %d\n", min, max);
        treeBalance(bst);
        min = max = -1;
        min = treeHeight(bst->root, &max, 1);
        printf("Post-balancing >\t Minimum path = %d\t Maximum path = %d\n", min, max);
    }
}

static int treeHeight(link h, int *max, int depth){
    int min_l, min_r;
    if(h == NULL) return 0;
    if(h->l == NULL || h->r == NULL){
        if(depth > *max) *max = depth;
        return 1;
    }
    min_l = treeHeight(h->l, max, depth+1);
    min_r = treeHeight(h->r, max, depth+1);
    if(h->l == NULL) return min_r + 1;
    if(h->r == NULL) return min_l + 1;
    return(min_l < min_r) ? min_l+1 : min_r+1;
}

static void treeBalance(BSTquotations bst){
    int k = (bst->count+1)/2 - 1;
    bst->root = treePartition(bst->root, k);
}

static link treePartition(link h, int k){
    int t = (h->l) ? h->l->cnt : 0;
    if(h == NULL) return NULL;
    if(t > k){
        h->l = treePartition(h->l, k);
        h = rotR(h);
    }
    if(t < k){
        h->r = treePartition(h->r, k-t-1);
        h = rotL(h);
    }
    return h;
}

link rotR(link h){
    link x = h->l;
    h->l = x->r;
    x->r = h;
    x->cnt = h->cnt;
    h->cnt = 1;
    h->cnt += (h->l) ? h->l->cnt : 0;
    h->cnt += (h->r) ? h->r->cnt : 0;
    return x;
}

link rotL(link h){
    link x = h->r;
    h->r = x->l;
    x->l = h;
    x->cnt = h->cnt;
    h->cnt = 1;
    h->cnt += (h->l) ? h->l->cnt : 0;
    h->cnt += (h->r) ? h->r->cnt : 0;
    return x;
}

void BSTgetMinMax(BSTquotations bst, float *min, float *max){
    if(BSTquotationsEmpty(bst)) return;
    minmax(bst->root, min, max);
}

static void minmax(link h, float *min, float *max){
    float q = QUOTATIONgetVal(h->q);
    if(h == NULL) return;
    if(*min < 0 || *min > q) *min = q;
    if(*max < 0 || *max < q) *max = q;
    if(h->r) minmax(h->r, min, max);
    if(h->l) minmax(h->l, min, max);
}