//
// Created by Alex on 04/07/2021.
//
#include <stdio.h>
#include <stdlib.h>
#include "headers/board.h"

bool is_possible_to_move(board_t pBoard, pawn_t pPawn, int x, int y);

bool is_possible_to_eat(board_t pBoard, pawn_t pPawn, int x, int y);

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

int eat(board_t board, pawn_t p, int x, int y) {
    if (p) {
        append(board->b[p->x][p->y], board->b[x + (p->x)][y + (p->y)]);
        board->b[x + (p->x)][y + (p->y)] = NULL;
        normal_move(board, board->b[p->x][p->y], x, y);
        return 1;
    }
    return 0;
}

int normal_move(board_t b, pawn_t p, int x, int y) {
    if (p) {
        b->b[x][y] = b->b[x-(x-p->x)][y-(y - p->y)];
        b->b[x-(x-p->x)][y-(y - p->y)] = NULL;
        p->x = x;
        p->y = y;

        while (p->next){
            p->next->x = x;
            p->next->y = y;
        }
        return 1;
    }
    return 0;
}

//todo bisogna vedere se è possibile muoversi e implementare 1. colore 2. re o no 3. se è possibile 4.non è fuori board
int is_possible_to_move(board_t b, pawn_t p, int x, int y) {
    return 0;
}


int is_possible_to_eat(board_t b, pawn_t p, int x, int y) {
    return 0;
}


//todo bisogna implementare la funzione per far decidere la mossa al backend
int move_factory(board_t b, pawn_t p, int x, int y){
    if(is_possible_to_move(b,p,x,y)){
        return normal_move(b,p,x,y);
    }else if(is_possible_to_eat(b,p,x,y))
        return eat(b,p,x,y);
    else
        return 0;
}



// ******** GRAPHICS ***************
void print_board(board_t *board) {
    int i, j;
    printf("  ");
    for (i = 0; i <= 27; i++) {
        if (i % 4 == 0) {
            printf("%d", i / 4);
        } else
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

char get_char(pawn_t pawn) {
    if (pawn) {
        if (pawn->color == BLUE) {
            if (pawn->status == GENERAL) {
                return 'B';
            } else return 'b';
        } else {
            if (pawn->status == GENERAL) {
                return 'R';
            } else return 'r';
        }
    }
    return ' ';
}

void print_board2(board_t *board) {
    int i, j;
    printf("  ");
    for (i = 0; i <= 27; i++) {
        if (i % 4 == 0) {
            printf("%d", i / 4);
        } else
            printf(" ");
    }
    printf("\n");
    for (i = 0; i < DIMENSION; i++) {
        printf("%d ", i);
        for (j = 0; j < DIMENSION; j++) {
            if ((*board)->b[i][j] == NULL)
                printf(" ");
            else {
                printf("%c", get_char((*board)->b[i][j]));

                if ((*board)->b[i][j]->next) {
                    printf("-%c", get_char((*board)->b[i][j]->next));

                    if ((*board)->b[i][j]->next->next) {
                        printf("-%c", get_char((*board)->b[i][j]->next));
                    }
                }
            }
            printf(" | ");
        }
        printf("\n");
    }
}



void test_for_piggies() {
    board_t b = init_board();
    pawn_t p = init_pawn(4, 2, BLUE, SOLDIER);
    normal_move(b,b->b[4][2],3,1);
    eat(b,b->b[2][0], 4,2);
    print_board2(&b);
    //printf("%d", count_stack(b->b[4][2]));
    delete_board(&b);
}