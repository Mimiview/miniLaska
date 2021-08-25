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


/* studia la legalità della mossa con salti di 1 o 2 caselle,
 * la prima condizione serve a vedere se la mossa entra nei range di mossa,
 * al suo interno lo statement studia la correttezza del movimento della pedina */
int is_a_legal_move(board_t b, pawn_t p, int x, int y) {
    if (b && p) {

        if (is_in_bounds(x, y) && ((p->x == x - 1 || p->x == x + 1 || p->x == x + 2 || p->x == x - 2) &&
                                   (p->y == y - 1 || p->y <= y + 1 || p->y == y + 2 || p->y <= y - 2))) {
            if (p->status == GENERAL)
                return 1;
            if (p->color == BLUE) {
                return (((x == p->x + 1) && (y == p->y - 1) || (x == p->x + 2) && (y == p->y - 2)) ||
                        ((x == p->x + 1) && (y == p->y + 1) || (x == p->x + 2) && (y == p->y + 2)));
            } else
                return (((x == p->x - 1) && (y == p->y - 1) || (x == p->x - 2) && (y == p->y - 2)) ||
                        ((x == p->x - 1) && (y == p->y + 1) || (x == p->x - 2) && (y == p->y + 2)));
        }
    } else
        return 0;
}

int is_possible_to_move(board_t b, pawn_t p, int x, int y) {
    if (b && p) {
        if (b->b[x][y] == NULL) {
            if (is_a_legal_move(b, p, x, y)) {
                return 1;
            } else
                return 0;
        } else
            return 0;
    }
}


int is_possible_to_eat(board_t b, pawn_t p, int x, int y) {
    if (is_possible_to_move(b, p, x, y)) {
        if (b->b[(p->x + x) / 2][(p->y + y) / 2] != NULL) {
            if (b->b[(p->x + x) / 2][(p->y + y) / 2]->color != p->color) {
                if (b->b[(p->x + x) / 2][(p->y + y) / 2]->status != GENERAL) {
                    return 1;
                } else
                    return p->status == GENERAL;
            }
            return 0;
        }
        return 0;
    }
}


// todo mangiare solo la testa della pedina, e mangiata obbligatoria
int eat(board_t board, pawn_t p, int x, int y) {
    if (p) {
        int x_food, y_food;
        x_food = (p->x + x) / 2; // con questa espressione troviamo la x e la y del mangiato
        y_food = (p->y + y) / 2;


        append(board->b[p->x][p->y], board->b[x_food][y_food]);
        if(count_stack(p)>=3)
            delete_last_pawn(p);
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
        if(p->color == BLUE && p->x == 6 || p->color == RED && p->x == 0)
            p->status = GENERAL;


        while (p->next) {
            p->next->x = x;
            p->next->y = y;
            p = p->next;
        }
        return 1;
    }
    return 0;
}


int move_factory(board_t b, pawn_t p, int x, int y) {

    if (is_possible_to_eat(b, p, x, y)) {
        return eat(b, p, x, y);
    } else if (is_possible_to_move(b, p, x, y))
        return normal_move(b, p, x, y);
    else
        return 0;
}
int paw_can_moves(board_t b, pawn_t p) {
    if (b && p)
        return (is_possible_to_move(b, p, p->x + 1, p->y + 1) || is_possible_to_move(b, p, p->x + 1, p->y - 1) ||
                is_possible_to_move(b, p, p->x + 2, p->y + 2) || is_possible_to_move(b, p, p->x + 2, p->y - 2) ||
                is_possible_to_move(b, p, p->x - 1, p->y - 1) || is_possible_to_move(b, p, p->x - 2, p->y - 2) ||
                is_possible_to_move(b, p, p->x - 1, p->y + 1) || is_possible_to_move(b, p, p->x - 2, p->y + 2));
    return 0;
}

int no_more_moves(board_t b, enum color_pawn color) {
    int flag;
    flag = 1;
    for (int i = 0; i < 6; ++i) {
        for (int j = 0; j < 6; ++j) {
            if (b->b[i][j] != NULL) // dovuto aggiungere perchè senno dava null pointer exception qunado andava
                if (b->b[i][j]->color == color && !paw_can_moves(b, b->b[i][j]))
                    flag = 0;
        }
    }
    return flag;
}

int no_more_paws(board_t b, enum color_pawn color) {

    for (int i = 0; i < 6; ++i) {
        for (int j = 0; j < 6; ++j) {
            if (b->b[i][j]->color == color)
                return 1;
        }
    }
    return 0;
}

int winner(board_t b, enum color_pawn color) {
    enum color_pawn enemy;
    if (color != RED)
        enemy = BLUE;
    else
        enemy = RED;


    if (no_more_paws(b, enemy) && no_more_moves(b, enemy))
        return 1;
    return 0;
}

//todo implementare la vittoria, implementare il conta stack e l'eliminazione di esso, implementare la possibilita di diventare generale(vedi dove conviene metterlo)
//todo vedere per quale motivo non funziona il movimento

//
// ******** GRAPHICS ***************

void print_pedina(pawn_t p) {
    if (p) {
        while (p) {
            if (p->color == BLUE) {
                if (p->status == GENERAL)
                    printf("B");
                else
                    printf("b");
            } else {
                if (p->status == GENERAL)
                    printf("R");
                else
                    printf("r");
            }
            p = p->next;
        }
    } else printf(" ");


}

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
            print_pedina((*board)->b[i][j]);
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
    move_factory(b, b->b[4][2], 3, 3);
    move_factory(b, b->b[2][4], 4, 2);
    move_factory(b, b->b[5][1], 3, 3);
    move_factory(b, b->b[3][3], 2, 4);
    move_factory(b, b->b[1][5], 3, 3);
    delete_last_pawn(b->b[3][3]);
    delete_pawn(b->b[6][0]);
    normal_move(b ,b->b[2][2], 6, 0);
    //eat(b, b->b[4][2], 2, 4);
    //
    print_board(&b);
    printf("pedine %d", count_stack(b->b[3][3]));
    //printf("%d ", is_possible_to_eat(b, b->b[1][5], 3, 3));
    //print_pedina(b->b[3][3]);
    //printf("%d mimmo", is_possible_to_move(b, b->b[5][1], 3, 4));
    //printf("%d", count_stack(b->b[4][2]));
    delete_board(&b);
}