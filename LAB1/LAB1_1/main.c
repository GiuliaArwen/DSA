#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define MAXDIM 100

int regexpLen(char *regexp);
int regexpCmpChar(char c, char *regexp, int *np);
int regexpCmp(char *s, char *regexp);
char *findRegExp(char *src, char *regexp);

int main(int argc, char **argv){
    char *src = argv[1], *regexp = argv[2], *exp;               // exp stores the pointer to the first occurrence
    int found = 0, len = regexpLen(regexp);
    
    printf("src = %s\n", src);
    printf("regexp = %s\n", regexp);
    printf("len_regexp = %d\n", len);

    do{
        exp = findRegExp(src, regexp);
        if(exp){
            printf("%.s\n", len, exp);                          // prints the matched substring
            src = exp+1;                                        // updates pointer to keep seeking matches
            found++;
        }
    } while (exp!=NULL);

    printf("%d matches found\n", found);

    return 0;
}

int regexpLen(char *regexp){                                    // regexpLen returns the right length for regexp according to special characters
    int i, len, jump;                                           // jump stores the number of characters to skip
    for(i = len = 0; regexp[i] != '\0'; len++, i += jump){
        regexpCmpChar(' ', &regexp[i], &jump);                  // space parameter as dummy to trigger function without comparison
    }
    return len;
}

int regexpCmpChar(char c, char *regexp, int *jump){
    int res, n;
    switch(regexp[0]){
        case '.':
            res = 1;
            n = 1;
            break;
        case '\\':
            res = (regexp[1] == 'a' ? islower(c) : isupper(c)); // on the assumption that the else statement is \A
            n = 2;
            break;
        case '[':
            res = 0;
            for(n=1; regexp[n] != ']'; n++){
                if(regexp[n] == '^') res = 1;                   // defines negation
                else if (regexp[n] == c) res = !res;            // equals 1 if match, equals 0 with negated match
            }
            n++;
            break;
        default:
            n=1;
            res = (c == regexp[0]);
    }
    *jump = n;
    return res;
}

int regexpCmp(char *s, char*regexp){
    int i, ir, jump;                                            // i is index for src, ir is index for regexp

    for(i=ir=0; regexp[ir] != '\0'; i++, ir += jump){
        if(!regexpCmpChar(s[i], &regexp[ir], &jump)) return 0;
    }
    return 1;
}

char *findRegExp(char *src, char *regexp){                        // wrapper function for comparison, returns pointer to first occurrence
    int len_src = strlen(src);
    int len_regexp = regexpLen(regexp);

    for(int i=0; i <= len_src - len_regexp; i++){
        if(regexpCmp(&src[i], regexp)) return(&src[i]);
    }
    return NULL;
}