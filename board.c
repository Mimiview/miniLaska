//
// Created by Alex on 04/07/2021.
//
#include <stdio.h>
#include <stdlib.h>
#include "headers/board.h"
#include "headers/pawn.h"

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
        int x_food, y_food;
        x_food = (p->x + x) / 2; // con questa espressione troviamo la x e la y del mangiato
        y_food = (p->y + y) / 2;

        append(board->b[p->x][p->y], board->b[x_food][y_food]);
        board->b[x_food][y_food] = NULL;
        normal_move(board, board->b[p->x][p->y], x, y);
        return 1;
    }
    return 0;
}

int normal_move(board_t b, pawn_t p, int x, int y) {
    if (p) {
        b->b[x][y] = b->b[x - (x - p->x)][y - (y - p->y)];
        b->b[x - (x - p->x)][y - (y - p->y)] = NULL;
        p->x = x;
        p->y = y;

        while (p->next) {
            p->next->x = x;
            p->next->y = y;
            p = p->next;
        }
        return 1;
    }
    return 0;
}

int is_legal_to_move(board_t b, pawn_t p, int x, int y) {
    if (b && p) {
        if(is_in_bounds(x, y) && (p->x-2))
            return 0; //todo da fare chiccooo
    }else
        return 1;
}

//todo deve finire di funzionare perchè manca l'implementazione di is_legal_to_move
int is_possible_to_move(board_t b, pawn_t p, int x, int y) {
    if (b && p) {
        if (b->b[x][y] == NULL) {
            if (is_legal_to_move(b,p,x,y)&&(p->status == GENERAL || (p->color == BLUE && p->y < y) || (p->color == RED && p->y > y))) {
                return 1;
            } else
                return 0;
        } else
            return 0;
    }
}


int is_possible_to_eat(board_t b, pawn_t p, int x, int y) {
    return 0;
}


//todo bisogna implementare la funzione per far decidere la mossa al backend
int move_factory(board_t b, pawn_t p, int x, int y) {
    if (is_possible_to_move(b, p, x, y)) {
        return normal_move(b, p, x, y);
    } else if (is_possible_to_eat(b, p, x, y))
        return eat(b, p, x, y);
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
    normal_move(b, b->b[2][4], 3, 3);
    eat(b, b->b[4][2], 2, 4);
    print_board2(&b);
    printf("%d mimmo", is_possible_to_move(b, b->b[1][3], 3, 5));
    //printf("%d", count_stack(b->b[4][2]));
    delete_board(&b);
}