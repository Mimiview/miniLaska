//
// Created by Alex on 04/07/2021.
//
#include <stdio.h>
#include <stdlib.h>
#include "headers/board.h"

board_t init_board() {
    int i, j;
    board_t board = (board_t) malloc(sizeof(struct board));
    if (board) {
        for (i = 0; i < DIMENSION; i++) {
            for (j = 0; j < DIMENSION; j++) {
                board->b[i][j] = NULL;
                if ((i + j) % 2 == 0) {
                    if (i < 3)
                        board->b[i][j] = init_pawn(i, j, BLUE, SOLDIER);
                    if (i > 3)
                        board->b[i][j] = init_pawn(i, j, RED, SOLDIER);
                }
            }
        }
        return board;
    }
    printf("malloc crushed");
    exit(1);

}

//da vedere se distruggerla del tutto oppure se porla a NULL
void delete_board(board_t *b) {
    int i, j;

    if (*b) {
        for (i = 0; i < DIMENSION; i++) {
            for (j = 0; j < DIMENSION; j++) {
                delete_pawn((*b)->b[i][j]);
            }
        }
        free(*b);
    }
}

int is_in_bounds(int x, int y) {
    return x >= 0 && x <= 6 && y >= 0 && y <= 6;
}

//TODO vedere se il giocatore può fare una mossa
int has_legal_moves(board_t board, pawn_t pawn) {

}

//ritorna 1 se la mossa è andata a buon fine, e zero se non è possibile farla
int move_pawn(board_t board, pawn_t pawn, int x, int y){
    if(has_legal_moves(board,pawn) && is_in_bounds(x, y)){
        board->b[pawn->x][pawn->y] = NULL;
        pawn->x = x;
        pawn->y = y;

        if( pawn->next ){
            pawn->next->x = x;
            pawn->next->y = y;
            if (pawn->next->next) {
                pawn->next->next->x = x;
                pawn->next->next->y = y;
            }
        }


        board->b[x][y] = pawn;
        return 1;
    }
    return 0;
}




void print_board(board_t *board) {
    int i, j;
    printf("  ");
    for (i = 0; i <= 27; i++) {
        if(i%4==0){
            printf("%d", i/4);
        }else
            printf(" ");
    }
    printf("\n");
    for (i = 0; i < DIMENSION; i++) {
        printf("%d ", i);
        for (j = 0; j < DIMENSION; j++) {
            if ((*board)->b[i][j] == NULL)
                printf(" ");
            else if ((*board)->b[i][j]->color == BLUE)
                printf("X");
            else if ((*board)->b[i][j]->color == RED)
                printf("O");
            printf(" | ");
        }
        printf("\n");
    }
}

char get_char(pawn_t pawn)
{
    if(pawn)
    {
        if(pawn->color == BLUE)
        {
            if(pawn->status == GENERAL)
            {
                return 'B';
            }
            else return 'b';
        }
        else
        {
            if(pawn->status == GENERAL)
            {
                return 'R';
            }
            else return 'r';
        }
    }
    return ' ';
}

void print_board2(board_t *board) {
    int i, j;
    printf("  ");
    for (i = 0; i <= 27; i++) {
        if(i%4==0){
            printf("%d", i/4);
        }else
            printf(" ");
    }
    printf("\n");
    for (i = 0; i < DIMENSION; i++) {
        printf("%d ", i);
        for (j = 0; j < DIMENSION; j++) {
            if ((*board)->b[i][j] == NULL)
                printf(" ");
            else
            {
                printf("%c",get_char((*board)->b[i][j]));

                if((*board)->b[i][j]->next)
                {
                    printf("-%c",get_char((*board)->b[i][j]->next));

                    if((*board)->b[i][j]->next->next)
                    {
                        printf("-%c",get_char((*board)->b[i][j]->next));
                    }
                }
            }
            printf(" | ");
        }
        printf("\n");
    }
}

void test_for_piggies(){
    board_t b = init_board();
    pawn_t p = init_pawn(4,2,BLUE,SOLDIER);
    append(&(b->b[0][0]), p);
    print_board2(&b);
    printf("%d", count_stack(b->b[4][2]));
    delete_board(&b);
}