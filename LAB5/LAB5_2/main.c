#include <stdio.h>
#include <stdlib.h>
#define FILENAME1 "tiles.txt"
#define FILENAME2 "board.txt"

typedef struct{
    char color[2];
    int value[2];
}tile_t;

typedef struct{
    int index;
    int rotation;
}board_t;

tile_t *readTiles(char *filename, int *N);
board_t **readBoard(char *filename, int *R, int *C, int *mark, int N);
void complete(board_t **board, tile_t *tile, int R, int C, int N, int *mark);
void completeR(int pos, board_t **board, tile_t *tile, int R, int C, int N, int *mark, int *bestScore, board_t **bestBoard);
int calculate(board_t **board, tile_t *tile, int R, int C, int N);

int main(void){
    int N=0, R=0, C=0;
    tile_t *tile;
    board_t **board;

    tile = readTiles(FILENAME1, &N);
    if(tile==NULL) return -1;
    int *mark = calloc(N, sizeof(int));                                                 // the size of the board is the same as the number of tiles
    board = readBoard(FILENAME2, &R, &C, mark, N);
    if(board==NULL) return -1;

    complete(board, tile, R, C, N, mark);
    return 0;
}

tile_t *readTiles(char *filename, int *N){
    tile_t *tile;
    FILE *fp = fopen(filename, "r");
    if(fp == NULL) exit(-1);

    fscanf(fp, "%d\n", N);                                                              // the '\n' is crucial!! Otherwise it will be read instead of the color
    tile = calloc(*N, sizeof(tile_t));
    for(int i=0; i<*N; i++) fscanf(fp, "%c %d %c %d\n", &tile[i].color[0], &tile[i].value[0], &tile[i].color[1], &tile[i].value[1]);

    fclose(fp);
    return tile;
}

board_t **readBoard(char *filename, int *R, int *C, int *mark, int N){
    board_t **board;
    FILE *fp = fopen(filename, "r");
    if(fp == NULL) exit(-1);

    fscanf(fp, "%d %d", R, C);
    board = calloc(*R, sizeof(board_t*));
    for(int i=0; i<*R; i++){
        board[i] = calloc(*C, sizeof(board_t));
        for(int j=0; j<*C; j++){
            fscanf(fp, "%d/%d", &board[i][j].index, &board[i][j].rotation);
            if(board[i][j].index >= 0 && board[i][j].index < N){                        // only checks tiles already placed
                if(mark[board[i][j].index]) return NULL;
                else mark[board[i][j].index] = 1;                                       // marks the tile as used
            }
        }
    }
    fclose(fp);
    return board;
}

void complete(board_t **board, tile_t *tile, int R, int C, int N, int *mark){           // wrapper function for optimal score
    int bestScore = -1;
    board_t **bestBoard = calloc(R, sizeof(board_t*));
    for(int i=0; i<R; i++) bestBoard[i] = calloc(C, sizeof(board_t));

    completeR(0, board, tile, R, C, N, mark, &bestScore, bestBoard);
    printf("Best solution: %d\n", bestScore);
    for(int i=0; i<R; i++){
        for(int j=0; j<C; j++)
            printf("%d/%d ", bestBoard[i][j].index, bestBoard[i][j].rotation);
        printf("\n");
    }
    printf("\n");
}

void completeR(int pos, board_t **board, tile_t *tile, int R, int C, int N, int *mark, int *bestScore, board_t **bestBoard){
    int score, row, col;
    if(pos >= R*C){                                                                      // exit condition: exceeded matrix size
        score = calculate(board, tile, R, C, N);                                         // for each complete board configuration, calculates score
        if(score > *bestScore){                                                          // updates best parameters
        *bestScore = score;
        for(int i=0; i<R; i++)
            for(int j=0; j<C; j++)
                bestBoard[i][j] = board[i][j];
       }
       return;
    }

    row = pos/C;                                                                        // formula to convert 1D indexes to 2D coordinates
    col = pos%C;

    if(board[row][col].index == -1)                                                     // checks for empty position
        for(int i=0; i<N; i++){                                                         // compares for each tile
            if(mark[i]) continue;                                                       // skips tile if taken
            mark[i] = 1;                                                                // otherwise marks it
            board[row][col].index = i;                                                  // chooses current tile
            board[row][col].rotation = 0;                                               // assumes 0 rotation
            completeR(pos+1, board, tile, R, C, N, mark, bestScore, bestBoard);
            board[row][col].rotation = 1;                                               // explores 1 rotation as well
            completeR(pos+1, board, tile, R, C, N, mark, bestScore, bestBoard);
            board[row][col].index = -1;                                                 // backtracks
            mark[i] = 0;
        }
    else completeR(pos+1, board, tile, R, C, N, mark, bestScore, bestBoard);            // if position not empty, explores next one
}

int calculate(board_t **board, tile_t *tile, int R, int C, int N){
    int sum = 0, part = 0, i, j;                                                        // variables to score the total and the partial score
    char first = '\0';

    for(i=0; i<R; i++){                                                                 // calculates row scoring
        first = '\0';
        part = 0;
        for(j=0; j<C; j++){
            if(board[i][j].index < 0 || board[i][j].index >= N) return -1;              // checks tile validity
            if(j == 0){
                first = tile[board[i][j].index].color[(0+board[i][j].rotation)%2];      // stores color of the first occurrence
                part = tile[board[i][j].index].value[(0+board[i][j].rotation)%2];       // stores its value
            }
            else{
                if(tile[board[i][j].index].color[(0+board[i][j].rotation)%2] == first)  // if the following colors match
                    part += tile[board[i][j].index].value[(0+board[i][j].rotation)%2];   // sums up their values
                else break;                                                             // otherwise skip row
            }
        }
        if(j == C) sum += part;                                                         // if it has succesfully reached the end of the row, saves the row score
    }
    for(j=0; j<C; j++){                                                                 // calculates column scoring
        first = '\0';
        part = 0;
        for(i=0; i<R; i++){
            if(board[i][j].index < 0 || board[i][j].index >= N) return -1;
            if(i == 0){
                first = tile[board[i][j].index].color[(1+board[i][j].rotation)%2];      // adds 1 to the index because it's considering the second pattern of the tile
                part = tile[board[i][j].index].value[(1+board[i][j].rotation)%2];
            }
            else{
                if(tile[board[i][j].index].color[(1+board[i][j].rotation)%2] == first)
                    part += tile[board[i][j].index].value[(1+board[i][j].rotation)%2];
                else break;
            }
        }
        if(i == R) sum += part;
    }
    return sum;
}