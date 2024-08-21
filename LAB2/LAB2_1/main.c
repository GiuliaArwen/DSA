#include <stdio.h>
#include <stdlib.h>

int gcd(int a, int b);
void swap(int *a, int *b);

int main(void){
    int a, b;

    printf("Enter two values a and b: ");
    scanf("%d %d", &a, &b);

    printf("\nGreatest common divisor for %d and %d is: %d.\n", a, b, gcd(a,b));

    return 0;
}

int gcd(int a, int b){
    if(a < b) swap(&a, &b);                         // assume a is greater than b
    if(a==b || b==0) return a;                      // condition to exit the recursion

    if(a%2 != 0){
        if(b%2 != 0) return gcd((a-b)/2, b);        // a odd, b odd case
        else return gcd(a, b/2);                    // a odd, b even case
    }
    else{
        if (b%2 == 0) return (2*gcd(a/2, b/2));     // a even, b even case
        else return gcd(a/2, b);                    // a even, b even case
    }
    return 0;
}

void swap(int *a, int *b){
    int tmp;
    tmp = *a;
    *a = *b;
    *b = tmp;
}